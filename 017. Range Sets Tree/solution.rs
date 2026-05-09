use std::io::{stdout, Write};

use self::segmap::SegMap;
use self::segtree::ImplicitSegTree;
pub use self::template::*;

mod segmap {
    use std::collections::BTreeSet;

    #[derive(Debug, Default, Clone, Ord, PartialOrd, Eq, PartialEq, Hash)]
    pub struct SegMap {
        map: BTreeSet<(usize, usize)>,
    }

    impl SegMap {
        #[allow(dead_code)]
        pub fn is_empty(&self) -> bool {
            self.map.is_empty()
        }

        pub fn insert(&mut self, l: usize, r: usize, mut callback: impl FnMut(usize, usize, i32)) {
            self.remove(l, r, &mut callback);
            self.map.insert((l, r));
            callback(l, r, 1);
        }

        pub fn remove(&mut self, l: usize, r: usize, mut callback: impl FnMut(usize, usize, i32)) {
            while let Some(&(ml, mr)) = self.map.range((l, 0)..(r, 0)).next() {
                self.map.remove(&(ml, mr));

                if mr <= r {
                    callback(ml, mr, -1);
                } else {
                    callback(ml, r, -1);
                    self.map.insert((r, mr));
                }
            }

            if let Some(&(ml, mr)) = self.map.range(..(r, 0)).next_back() {
                assert!(ml < l);

                if l < mr {
                    self.map.remove(&(ml, mr));
                    callback(l, mr.min(r), -1);
                    self.map.insert((ml, l));
                    if r < mr {
                        self.map.insert((r, mr));
                    }
                }
            }
        }
    }
}

mod segtree {
    use std::collections::HashMap;
    use std::ops::{AddAssign, Neg};

    #[derive(Debug, Default, Clone, Eq, PartialEq)]
    pub struct ImplicitSegTree<T: Copy + Default + Neg<Output = T> + AddAssign> {
        len: usize,
        arr: HashMap<usize, T>,
    }

    impl<T: Copy + Default + Neg<Output = T> + AddAssign> ImplicitSegTree<T> {
        pub fn new(n: usize) -> Self {
            Self {
                len: n + 1,
                arr: Default::default(),
            }
        }

        fn add(&mut self, mut wh: usize, x: T) {
            wh += self.len;

            while wh > 0 {
                *self.arr.entry(wh).or_default() += x;
                wh /= 2;
            }
        }

        pub fn segadd(&mut self, l: usize, r: usize, x: T) {
            self.add(l, x);
            self.add(r, -x);
        }

        pub fn get(&self, wh: usize) -> T {
            let mut l = self.len;
            let mut r = wh + self.len + 1;

            let mut sum = Default::default();

            while l < r {
                if l % 2 != 0 {
                    sum += self.arr.get(&l).copied().unwrap_or_default();
                    l += 1;
                }
                if r % 2 != 0 {
                    r -= 1;
                    sum += self.arr.get(&r).copied().unwrap_or_default();
                }

                l /= 2;
                r /= 2;
            }

            sum
        }
    }
}

fn solve_test(scan: &mut impl Scanner) {
    let n = scan.next();
    let q = scan.next();

    let mut layers = vec![SegMap::default(); q];
    let mut sums = ImplicitSegTree::new(n);

    for _query in 0..q {
        let ch: char = scan.next();

        match ch {
            '+' | '-' => {
                let l = scan.next::<usize>() - 1;
                let r = scan.next();
                let x = scan.next::<usize>() - 1;

                let callback = |l: usize, r: usize, x: i32| {
                    sums.segadd(l, r, x);
                };

                if ch == '+' {
                    layers[x].insert(l, r, callback);
                } else {
                    layers[x].remove(l, r, callback);
                }
            }

            '?' => {
                let k = scan.next::<usize>() - 1;
                println!("{}", sums.get(k));
                stdout().lock().flush().unwrap();
            }

            _ => unreachable!(),
        }
    }
}

pub fn solve(scan: &mut impl Scanner) {
    let t = 1;

    for _test in 0..t {
        solve_test(scan);
    }
}

#[allow(dead_code)]
fn main() {
    solve(&mut IOScanner::new(std::io::stdin().lock()))
}

pub mod template {
    use std::io::BufRead;

    pub trait Scanner {
        fn next<T: std::str::FromStr>(&mut self) -> T;

        fn seek(&mut self) -> bool;
    }

    pub struct IOScanner<ReadType: BufRead> {
        read: ReadType,
        buffer: Vec<String>,
    }

    impl<ReadType: BufRead> Scanner for IOScanner<ReadType> {
        fn next<T: std::str::FromStr>(&mut self) -> T {
            loop {
                if let Some(token) = self.buffer.pop() {
                    return token.parse().ok().expect("Failed parse");
                }
                let mut input = String::new();
                self.read.read_line(&mut input).expect("Failed read");
                self.buffer = input.split_whitespace().rev().map(String::from).collect();
            }
        }

        fn seek(&mut self) -> bool {
            let mut input = String::new();

            while let Ok(cnt) = self.read.read_line(&mut input) {
                if cnt == 0 {
                    break;
                }
                let line = input.trim();
                if !line.is_empty() && line.matches("=").count() == line.len() {
                    return true;
                }
                input.clear();
            }

            return false;
        }
    }

    impl<ReadType: BufRead> IOScanner<ReadType> {
        pub fn new(read: ReadType) -> IOScanner<ReadType> {
            IOScanner {
                read,
                buffer: Vec::<String>::default(),
            }
        }
    }
}
