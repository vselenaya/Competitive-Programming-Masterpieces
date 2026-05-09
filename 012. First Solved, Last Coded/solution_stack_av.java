/*
File name: stack_av.java
Tag: MAIN
Author: avasilyev
Change time: Sun Oct 29 05:53:29 MSK 2023
*/

import java.io.*;
import java.util.*;

public class stack_av {

    void solve() {
        int n = in.nextInt();
        int[] a = in.nextIntArray(n);
        int[] b = in.nextIntArray(n);
        checkSameValues(a, b);
        int[][][] posOfFirst = new int[n + 1][n + 1][n + 1];
        for (int i = 0; i <= n; i++) {
            for (int j = 0; j <= n; j++) {
                Arrays.fill(posOfFirst[i][j], -1);
            }
        }
        for (int i = 0; i <= n; i++) {
            for (int j = 0; j <= n; j++) {
                posOfFirst[0][i][j] = 0;
            }
        }
        for (int len = 1; len <= n; len++) {
            for (int i = 0; i + len <= n; i++) {
                for (int j = 0; j + len <= n; j++) {
                    for (int matchI = 0; matchI < len; matchI++) {
                        if (a[i] != b[j + matchI]) continue;
                        int len1 = matchI, len2 = len - 1 - matchI;
                        if (posOfFirst[len1][i + 1][j] != -1 && posOfFirst[len2][i + matchI + 1][j + matchI + 1] != -1) {
                            posOfFirst[len][i][j] = matchI;
                            break;
                        }
                    }
                }
            }
        }
        if (posOfFirst[n][0][0] == -1) {
            out.println("NO");
            return;
        }
        out.println("YES");
        restoreAns(posOfFirst, n, 0, 0);
        out.println();
    }

    private void checkSameValues(int[] a, int[] b) {
        a = a.clone();
        b = b.clone();
        Arrays.sort(a);
        Arrays.sort(b);
        if (!Arrays.equals(a, b)) {
            throw new AssertionError();
        }
    }

    void restoreAns(int[][][] posOfFirst, int len, int i, int j) {
        if (len == 0) return;
        int pos = posOfFirst[len][i][j];
        out.print("S");
        restoreAns(posOfFirst, pos, i + 1, j);
        out.print("C");
        restoreAns(posOfFirst, len - pos - 1, i + pos + 1, j + pos + 1);
    }

    void run() {
        in = new FastScanner(System.in);
        out = new PrintWriter(System.out);
        solve();
        out.close();
    }

    FastScanner in;
    PrintWriter out;

    public static void main(String[] args) {
        new stack_av().run();
    }

    public class FastScanner {
        public BufferedReader br;
        public StringTokenizer st;

        public FastScanner(InputStream in) {
            br = new BufferedReader(new InputStreamReader(in));
        }

        public FastScanner(String fileName) {
            try {
                br = new BufferedReader(new FileReader(fileName));
            } catch (FileNotFoundException e) {
                e.printStackTrace();
            }
        }

        public int nextInt() {
            return Integer.parseInt(next());
        }

        public boolean hasMoreTokens() {
            while (st == null || !st.hasMoreElements()) {
                String line = null;
                try {
                    line = br.readLine();
                } catch (IOException e) {
                    return false;
                }
                if (line == null) {
                    return false;
                }
                st = new StringTokenizer(line);
            }
            return true;
        }

        public String next() {
            while (st == null || !st.hasMoreElements()) {
                String line = null;
                try {
                    line = br.readLine();
                } catch (IOException e) {
                }
                st = new StringTokenizer(line);
            }
            return st.nextToken();
        }

        public long nextLong() {
            return Long.parseLong(next());
        }

        public double nextDouble() {
            return Double.parseDouble(next());
        }

        public int[] nextIntArray(int n) {
            int[] ret = new int[n];
            for (int i = 0; i < n; i++) {
                ret[i] = nextInt();
            }
            return ret;
        }

        public long[] nextLongArray(int n) {
            long[] ret = new long[n];
            for (int i = 0; i < n; i++) {
                ret[i] = nextLong();
            }
            return ret;
        }
    }
}
