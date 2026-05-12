#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

struct Event_vert {  // структура для хранения вертикальных сторон прямоугольника
    int x, y1, y2, type;  // type - тип (=1 для левой и -1 для правой сторон)
    
    Event_vert(int x, int y1, int y2, int type)
        : x(x), y1(y1), y2(y2), type(type)
    {}
};

struct Event_hor {  // для горизонтальных сторон прямоугольника
    int y, x1, x2, type;  // type = 1 для нижней (с меньшим y) и -1 для верхней сторон
    
    Event_hor(int y, int x1, int x2, int type)
        : y(y), x1(x1), x2(x2), type(type)
    {}
};

bool cmp_vert(Event_vert a, Event_vert b) {  // сортируем вертикальные стороны слева направо
    if (a.x < b.x)
        return 1;
    
    if (a.x > b.x)
        return 0;
    
    return (a.type > b.type);
}

bool cmp_hor(Event_hor a, Event_hor b) {  // горизонтальны снизу вверх
    if (a.y < b.y)
        return 1;
    
    if (a.y > b.y)
        return 0;
    
    return (a.type > b.type);
}

int main() {
    int N;
    cin >> N;
    
    // для решения задача используем две сканирующие прямые: первая идёт слева направо по вертикальным сторонам
    // прямогольников -> эти вертикальны стороны храним в векторе:
    vector <Event_vert> segments;
    
    // вторая сканирующая прмяая будет идти снизу вверх по y-координтам, подсчитывая суммарную толщину
    // открытых (для первой сканирующей прямой) прямоугльникв -> y-координаты тоже в векторе:
    vector <Event_hor> points;
    
    for (int i = 0; i < N; i ++) {
        int x1, x2, y1, y2;
        cin >> x1 >> y1 >> x2 >> y2;
        
        segments.push_back(Event_vert(x1, y1, y2, 1));  // открывающая вертикальная сторона
        segments.push_back(Event_vert(x2, y1, y2, -1));
        points.push_back(Event_hor(y1, x1, x2, 1));  // открывающая нижняя сторона
        points.push_back(Event_hor(y2, x1, x2, -1));
    }
    sort(segments.begin(), segments.end(), cmp_vert);  // сортируем в поярдке обхода сканирующими прямыми
    sort(points.begin(), points.end(), cmp_hor);
    
    int balance = 0;  // количество открытых прямойгольников при обходе первой сканирующей прямой слева направо
    long long sum_square = 0;  // суммарная площать объединения прямоугольников
    for (int i = 0; i < segments.size(); i ++) {
        // итак, первая сканирующая прямая наткнулась на i-ую вертикальную сторону прямоугольника
        
        if (balance > 0) {  // если сейчас balance > 0, значит от i-1-ой до текущей i-ой вертикальной стороны
                            // (эти вертикальны стороны находятся в segments[i-1].x и segments[i].x по x-координате)
                            // находились (были открыты) прямоугольники -> посчитаем их суммарную толщину:
            
            // для нахождения суммарной толщины открытых (между segments[i-1].x и segments[i].x - это
            // x-координаты вертикальных сторон, между которыми нас сейчас интересуют прямоугольники)        
            // прямоугольников, пробежимся снизу вверх сканирующей прямой по y-координатам (по горизонтальным
            // отрезкам) и подсчитаем длину (по вертикали) объединения открытых прямоугольников:
            int vert_bal = 0, vert_line = 0, prev_y;
            for (int j = 0; j < points.size(); j ++) {                
                // игнорируем те горизонатльные отрезки, которые не находятся между segments[i-1].x и segments[i].x:
                if (!(points[j].x1 <= segments[i-1].x && segments[i].x <= points[j].x2))
                    continue;
                if (vert_bal > 0)  // если vert_bal > 0, значит от предыдущей prev_y то текущей points[j].y 
                                   // координат были открыты прямоугольники -> добавляем толщину:
                    vert_line += points[j].y - prev_y;
                vert_bal += points[j].type;
                prev_y = points[j].y;  // фиксируем теперь уже предыдущую y-координату
            }
            
            // добавляем к площади пройденную первой сканирующей прямой длину, умноженную на суммарную толщину:
            sum_square += 1ll * (segments[i].x - segments[i-1].x) * vert_line;
        }
        
        balance += segments[i].type;  // обновляем
    }
    
    cout << sum_square << endl;
    
    return 0;
    
}
