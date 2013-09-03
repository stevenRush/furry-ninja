using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Collections;

namespace Prim
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)
        {
           
            // Получаем количество узлов
            int k = int.Parse(textBox1.Text);
            if (k < 2)
            {
                MessageBox.Show("Слишком мало узлов!");
                return;
            }
            ForImage.msm = new string[k];
            ForImage.mves = new string[k];
            // Создаем граф
            Graph gf = new Graph(k);
            // Подсчитываем вес ребер
            gf.CalcWeights();
            // Строим минимально дерево на графе
            gf.Prim();
            // Рисуем на полотне граф
            gf.Draw();
            // Отображаем на экране полотно 
            textBox2.Lines = ForImage.mves;
            textBox3.Lines = ForImage.msm;
            gf.GetMaxPath();
            pictureBox1.Image = ForImage.img;
            ForImage.img = new Bitmap(1423, 533);
        }

        private void pictureBox1_Click(object sender, EventArgs e)
        {

        }

        private void Form1_Load(object sender, EventArgs e)
        {

        }

    }


    public class ForImage
    {
        public static Image img = new Bitmap(1423, 533);
        public static string[] mves;
        public static string[] msm;
    }


    // Класс Node - узел графа. 
    // Хранит в себе координату узла.
    class Node
    {
        // Координаты
        public int x;   
        public int y;

        // Конструктор
        public Node(int x, int y)
        {
            this.x = x;
            this.y = y;
        }

        // Метод, возвращающий координаты
        public void GetCoord(out int x, out int y)
        {
            x = this.x;
            y = this.y;
        }

        // Метод отрисовки узла
        public void Draw()
        {
            // Устанавливаем кисть
            SolidBrush blueBrush = new SolidBrush(Color.Blue);
            Graphics g = Graphics.FromImage(ForImage.img);
            // Рисуем круг радиусом 3 (узел)
            g.FillEllipse(blueBrush, x - 3, y - 3, 6, 6);
        }         
    }

    
    // Класс NodeList
    // Представляет собой список узлов 
    class NodeList
    {
        // Массив узлов
        Node[] a;
        // Их максимальное количество
        int kolvo;

        // Конструктор
        public NodeList(int k)
        {
            kolvo = k;
            // Выделяем память под массив узлов
            a = new Node[kolvo];
        }

        // Метод, освобождающий память после работы
        public void Done()
        {
            for(int i=0; i < Count(); i++)
                a[i] = null;
            a = null;
        }

        // Метод, добавляющий узел в список
        public void Add(Node p)
        {
            // Вычисляем количество уже добавленных узлов
            int c = Count()-1,
                x1, x2, y1, y2;
            // Если это количество превышает размер списка, то выходим
            if (c >= kolvo)
                return;
            // Поиск совпадений среди узлов. Если узел с такими координатами уже существует, то нет смысла его добавлять
            for (int i = 0; i < c; i++)
            {
                if (a[i] == null)
                    continue;
                a[i].GetCoord(out x1, out y1);
                p.GetCoord(out x2, out y2);
                if (x1 == x2 && y1 == y2)
                    return;
            }
            // Добавляем узел в конец списка
            a[c + 1] = p;            
        }

        // Удаление узла с номером num
        public void Delete(int num)
        {
            // Если узла с данным номером не существует, то выходим
            if (num > kolvo || a[num] == null)
                return;
            // Подсчитываем количество узлов в списке
            int c = Count();
            // Удаляем данный узел
            a[num] = null;
            // Сдвигаем список на одну позицию для убирания дырки
            for (int i = num; i < c - 1; i++)
                a[i] = a[i + 1];
            // Удаляем последний элемент, т.к. он скопирован на место предыдущего
            a[c] = null;
        }

        // Метод, отрисовывающий все узлы списка
        public void DrawAll()
        {
            // Получаем количество узлов
            int c = Count();
            // Рисуем их по очереди 
            for (int i = 0; i < c; i++)
                a[i].Draw();
        }

        // Получает координаты узла с номером num
        public void GetCoord(int num, out int x, out int y)
        {
            x = 0;
            y = 0;
            // Проверяем, существует ли такой узел
            if (num > kolvo || a[num] == null)
                return;
            // Запрашиваем координаты у данного узла
            a[num].GetCoord(out x, out y);
        }

        // Подсчет узлов в списке
        public int Count()
        {
            int k=0;
            for (int i = 0; i < kolvo; i++)
                if (a[i] != null)
                    k++;
            return k;
        }
    }


    // Класс WeightMatr.
    // Используется для хранения веса ребер
    class WeightMatr
    {
        // Двумерный массив, хранящий вес ребер
        double[][] Vesa;
        // Количество узлов, информация о которых хранится в матрице
        int n;

        // Конструктор
        public WeightMatr(int num)
        {
            n = num;
            // Выделяем память под матрицу весов
            Vesa = new double[num][];
            for (int i = 0; i < num; i++)
                Vesa[i] = new double[i+1];
        }

        // Подсчет веса ребер
        public void Calc(NodeList a)
        {
            int x1, x2, y1, y2;
            double r;
            // Перебираем все ребра
            for (int i = 0; i < n; i++)
            {
                // Получаем координаты первого узла
                a.GetCoord(i, out x1, out y1);
                for (int j = 0; j < i; j++)
                {
                    // Получаем координаты второго узла
                    a.GetCoord(j, out x2, out y2);
                    // Подсчитываем вес ребра (расстояние между узлами)
                    r = Math.Sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
                    // Записываем его в матрицу
                    Vesa[i][j] = r;
                }
            }
        }

        // Метод, возвращающий элемент с индексами i, j
        public double Get(int i, int j)
        {
            int t;
            // Расстояние между одним и тем же узлов равно нулю 
            if (i == j)
                return 0;
            if (j > i)
            {
                t = i;
                i = j;
                j = t;
            }
            if (Vesa[i][j] < 0)
                return 0;
            // Возвращаем запрашиваемое значение
            return Vesa[i][j];
        }

        // Освобождение памяти
        public void Done()
        {
            for (int i = 0; i < n; i++)
                Vesa[i] = null;
            Vesa = null;
        }
    }


    // Класс IntcidentMatr - Матрица смежности
    // Хранит информацию о смежности узлов
    class IntcidentMatr
    {
        // Объявляем матрицу как объект класса KNUT
        // Это очень удобно, т.к. получившаяся матрица будет разреженной
        KNUT k;

        // Конструктор
        public IntcidentMatr(int num)
        {
            // Инициализируем матрицу
            k = new KNUT(num, num);
        }

        // Метод, освобождающий память
        public void Done()
        {
            k.Done();
        }

        // Метод, записывающий элемент в матрицу
        public void Put(int i, int j, int val)
        {
            int t;
            if (j > i)
            {
                t = i;
                i = j;
                j = t;
            }
            k.Put(i, j, val);
        }

        // Метод, получающий значение из матрицы
        public int Get(int i, int j)
        {
            int t;
            if (j > i)
            {
                t = i;
                i = j;
                j = t;
            }
            return k.Get(i, j);
        }
    }


    // Класс Graph. 
    // Собственно, хранит в себе всю информацию о графе
    class Graph
    {

        // Список узлов
        NodeList l;
        int max;
        // Матрица веса
        WeightMatr v;
        // Матрица смежности
        IntcidentMatr Int;
        // Количество узлов
        int kolvo;

        // Конструктор
        public Graph(int kolvo)
        {
            Node p;
            this.kolvo = kolvo;
            Random rnd = new Random();
            l = new NodeList(kolvo);
            int x, y;
            // Создаем узлы в случайных точках и добавляем в список
            for (int i = 0; i < kolvo; i++)
            {
                x = rnd.Next(634) + 4;
                y = rnd.Next(474) + 4;
                p = new Node(x, y);
                l.Add(p);
            }
        }
        
        private double Dj(int s, out int[] mas)
        {
            ArrayList lst = new ArrayList();
            int i, min, n;
            double k;
            int[][] arr = new int[kolvo][];
            double [] f = new double[kolvo];
            for(i = 0; i < kolvo; i++)
            {
                lst.Add(i);
                f[i] = double.MaxValue;
                arr[i] = new int[1];
                arr[i][0] = s;
            }
            f[s] = 0;
            while(lst.Count > 0)
            {
                min = 0;
                 for(i = 1; i<kolvo; i++)
                     if(f[i]<f[min] && lst.Contains(i) || !lst.Contains(min))
                         min = i;
                 for(i=0; i<kolvo; i++)
                     if (Int.Get(min, i) == 1)
                     {
                         k=v.Get(min, i);
                         if (f[i] > f[min] + k)
                         {
                             f[i] = f[min] + k;
                             n = arr[min].Length;
                             arr[i] = new int[n + 1];
                             arr[min].CopyTo(arr[i], 0);
                             arr[i][n] = min;
                         }
                     }
                 lst.Remove(min);
             }
             max = 0;
             for (i = 1; i < kolvo; i++)
                 if (f[i] > f[max])
                     max = i;
             mas = arr[max];
             Array.Resize(ref mas, mas.Length + 1);
             mas[mas.Length-1] = max;
             return f[max];
         }

         public void GetMaxPath()
         {
             int maxin = 0; 
             int maxjn = 0;
             double max = 0;
             int[] mas, mas1;
             mas1 = new int[1];
             for (int i = 0; i < kolvo; i++)
             {
                 double k = Dj(i, out mas);
                 if (k > max)
                 {
                     max = k;
                     maxin = this.max;
                     maxjn = i;
                     mas1 = new int[mas.Length];
                     mas.CopyTo(mas1, 0);
                 }
             }
             Graphics g = Graphics.FromImage(ForImage.img);
             SolidBrush br = new SolidBrush(Color.Red);
             Pen pn = new Pen(Color.Red, 2);
             int x1, x2, y1, y2;
             for (int j = 0; j < mas1.Length - 1; j++)
             {
                 if (Int.Get(mas1[j], mas1[j + 1]) == 0)
                     continue;
                 l.GetCoord(mas1[j], out x1, out y1);
                 l.GetCoord(mas1[j+1], out x2, out y2);
                 g.DrawLine(pn, x1, y1, x2, y2);
             }

             l.GetCoord(maxin, out x1, out y1);
             l.GetCoord(maxjn, out x2, out y2);
             g.FillEllipse(br, x1 - 4, y1 - 4, 8, 8);
             g.FillEllipse(br, x2 - 4, y2 - 4, 8, 8);
         }

    



        // Метод, освобождающий память
        public void Done()
        {
            l.Done();
            v.Done();
            Int.Done();
        }

        // Метод формирования матрицы веса
        public void CalcWeights()
        {
            v = new WeightMatr(kolvo);
            v.Calc(l);
        }

        // Главный алгоритм построения минимального остовного дерева на графе
        public void Prim()
        {
            // Массив, хранящий номера уже использованных узлов
            DinVect used;
            int imin,   // Номер строки с минимальным элементом
                jmin,   // Номер столбца с минимальным элементом
                i, j, 
                cnt,    // Количество узлов
                k;      // Количество пройденных ребер
            double min, r;
            cnt = kolvo;
            imin = 0;
            jmin = 0;
            // Инициализация массива и матрицы смежности
            used = new DinVect(cnt);
            Int = new IntcidentMatr(cnt);
            min = 1000000;
            // Ищем ребро с минимальным весом
            for(i=0; i < cnt; i++)
                for (j = 0; j < cnt; j++)
                {
                    r = v.Get(i, j);
                    if (r != 0 && r < min)
                    {
                        min = r;
                        imin = i;
                        jmin = j;
                    }
                }
            // Формируем массив использованных элементов
            for (i = 0; i < cnt; i++)
                used.Put(i, i);
            // Записываем первоначальное значение в матрицу смежности
            Int.Put(imin, jmin, 1);
            // Указываем, что два элемента уже 
            used.Put(imin, -1);
            used.Put(jmin, -1);
            k = 2;
            // Пока не переберем все ребра, делаем:
            while (k != cnt)
            {
                min = 10000000;
                // Поиск следующего минимального еще незадействованного ребра
                for (i = 0; i < cnt; i++)
                {
                    if (used.Get(i) == -1)
                        continue;
                    for (j = 0; j < cnt; j++)
                    {
                        r = v.Get(i, j);
                        if (r != 0 && r < min && used.Get(j) == -1)
                        {
                            min = r;
                            imin = i;
                            jmin = j;
                        }
                    }
                }
                // Указываем, что найденное ребро смежно с ребром, уже находящимся в дереве
                Int.Put(imin, jmin, 1);
                // Указываем, что данное ребро уже использовано
                used.Put(imin, -1);
                // Увеличиваем счетчик использованных ребер
                k++;
            }
        }

        // Метод отрисовки всех узлов и связей между ними
        public void Draw()
        {
            int x1, x2, y1, y2, i, j;
            // Создаем полотно для рисования
            Graphics g = Graphics.FromImage(ForImage.img);
            // Создаем кисть для рисования
            Pen bluePen = new Pen(Color.Blue, 2);
            // Рисуем все узлы
            l.DrawAll();
            for (i = 0; i < kolvo; i++)
                for (j = 0; j < kolvo; j++)
                {
                    // Рисуем связи между смежными ребрами
                    if (Int.Get(i, j) == 1)
                    {
                        l.GetCoord(i, out x1, out y1);
                        l.GetCoord(j, out x2, out y2);
                        g.DrawLine(bluePen, x1, y1, x2, y2);
                    }
                }

            for (i = 0; i < kolvo; i++)
            {
                ForImage.mves[i] = "";
                ForImage.msm[i] = "";
                for (j = 0; j < kolvo; j++)
                {
                    ForImage.mves[i] += String.Format("{0,6:##0.00}", v.Get(i, j)) + "  ";
                    ForImage.msm[i] += String.Format("{0}", Int.Get(i, j)) + "  ";
                }
            }

        }
    }
    class KNUT
    {
        int NumRow,         // Количество строк
            NumCol,         // Количество столбцов
            Count;          // Количество элементов в матрице
        DinVect AN,         // Вектор, хранящий значения элементов
                AI,         // Вектор, хранящий номер строки каждого элемента   
                AJ,         // Вектор, хранящий номер столбца каждого элемента   
                NR;         // Вектор, хранящий номера следующих элементов в данной строке
        int[] JR;          // Массив точек входа в строки

        // Конструктор
        // i: Количество строк
        // j:Количество столбцов
        public KNUT(int i, int j)
        {
            NumRow = i;
            NumCol = j;
            Count = -1;

            // Инициализируем векторы
            AN = new DinVect(NumRow * NumCol);
            AI = new DinVect(NumRow * NumCol);
            AJ = new DinVect(NumRow * NumCol);
            NR = new DinVect(NumRow * NumCol);

            // Инициализируем массив 
            JR = new int[NumRow];
            for (int _i = 0; _i < NumRow; _i++)
                JR[_i] = -1;
        }

        // Помещает элемент в матрицу
        // i: Номер строки
        // j: Номер столбца
        // val: Значение, которое надо поместить
        public void Put(int i, int j, int val)
        {
            int pred, pos, sled;

            // Если передан 0, то нужно удалить элемент с данной позиции
            if (val == 0)
            {
                Delete(i, j);
                return;
            }

            // Определяем "соседей" данного элемента по строке
            FindRow(i, j, out pos, out pred, out sled);
            // Если такой элемент уже существует, то
            if (pos != -1)
            {
                // Заменяем значение на переданное и выходим. Никаких изменений в структуре делать не надо
                AN.Put(pos, val);
                return;
            }

            // Увеличиваем счетчик элементов
            pos = ++Count;
            // Если элемент первый в строке - то делаем его точкой входа в строку
            if (pred == -1)
                JR[i] = pos;
            // Иначе указываем предыдущему элементу, то данный элемент - следующий
            else
                NR.Put(pred, pos);

            // Указываем следующий, по отношению к данному, элемент
            NR.Put(pos, sled);
            // Заносим номер строки
            AI.Put(pos, i);
            // Заносим номер столбца
            AJ.Put(pos, j);
            // Заносим значение элемента
            AN.Put(pos, val);
        }

        // Возвращает элемент матрицы
        // i: Номер строки
        // j: Номер столбца
        public int Get(int i, int j)
        {
            // Если в данной строке нет элементов, то возвр ноль
            if (JR[i] == -1)
                return 0;
            int pred, pos, sled;
            // Определяем позицию элемента
            FindRow(i, j, out pos, out pred, out sled);
            // Если позиция не указана, то на этом месте стоит 0
            if (pos == -1)
                return 0;
            // Иначе возвращаем запрашиваемый элемент
            return AN.Get(pos);
        }
      
        // Отображение матрицы и векторов на экране
        public void Print()
        {
            for (int i = 0; i < NumRow; i++)
            {
                for (int j = 0; j < NumCol; j++)
                    Console.Write("{0} ", Get(i, j));
                Console.WriteLine();
            }
            Console.WriteLine();
            Console.Write("AN: ");
            AN.Print();
            Console.Write("AI: ");
            AI.Print();
            Console.Write("AJ: ");
            AJ.Print();
            Console.Write("NR: ");
            NR.Print();
        }
      
        // Удаление элемента
        /// i: Номер строки
        /// j: Номер столбца
        public void Delete(int i, int j)
        {
            int pos, pred, sled, t;
            // Определяем позицию данного элемента
            FindRow(i, j, out pos, out pred, out sled);
            // Если позиция пуста, то выходим
            if (pos == -1)
                return;
            // Если элемент первый в строке - нужно изменить точку входа
            if (pred == -1)
                JR[i] = sled;
            // Иначе нужно указать предыдущему элементу, что его "следующим" элементом будет теперь следующий по отношению к удаляемому
            else
                NR.Put(pred, sled);
            // Раз освободилась ячейка, то нужно ее заполнить. Записываем в данную ячейку последний элемент, тем самым уменьшая векторы
            // Переносим значение 
            AN.Put(pos, AN.Get(Count));
            // Запоминаем номер строки последнего элемента
            i = AI.Get(Count);
            // И номер столбца
            j = AJ.Get(Count);
            // Записываем их в новую ячейку (которая освободилась)
            AI.Put(pos, i);
            AJ.Put(pos, j);
            // Переносим также номер "следующего" элемента
            NR.Put(pos, NR.Get(Count));
            // Определяем "соседей" последнего элемента
            FindRow(i, j, out t, out pred, out sled);
            // И указываем им новую позицию этого элемента
            if (pred == -1)
                JR[i] = pos;
            else
                NR.Put(pred, pos);
            // Уменьшаем счетчик элементов
            Count--;
        }

        // Определение позиции данного элемента, а также его "соседей"
        // i: Номер строки
        // j: Номер столбца
        // pos: Позиция элемента в векторах (выходное значение)
        // pred: Номер предыдущего элемента (выходное значение)
        // sled: Номер следующего элемента (выходное значение)
        public void FindRow(int i, int j, out int pos, out int pred, out int sled)
        {
            // Получаем точку входа
            int perv = JR[i];
            pred = -1;
            sled = -1;
            pos = -1;
            // Если элементов в данной строке нет, то выходим
            if (perv == -1)
                return;

            // Если первый элемент уже стоит дальше, чем искомый, то помечаем первый элемент как "следующий" и выходим
            if (AJ.Get(perv) > j)
            {
                sled = perv;
                return;
            }

            // Цикл перебора всех элементов строки
            sled = perv;
            while (sled != -1)
            {
                // Если следующий элемент находится правее, чем искомый, то поиск окончен и выходим
                if (sled != -1 && AJ.Get(sled) > j)
                    return;
                // Если такой элемент уже существует, то запоминаем его позицию в переменной pos и выходим
                if (sled != -1 && AJ.Get(sled) == j)
                {
                    pos = sled;
                    sled = NR.Get(pos);
                    return;
                }
                // Переходим к следующей паре значений
                pred = sled;
                sled = NR.Get(pred);
            }
        }


        // Освобождение памяти
        public void Done()
        {
            AN.Done();
            AI.Done();
            AJ.Done();
            NR.Done();
            JR = null;
        }
    }


    class DinVect
    {
        /* Класс динамический вектор. Для экономии памяти сделаем следующее:
         * Разобъем вектор на блоки размером 16 элементов.
         * Изначально память не выделяется ни под один блок.
         * При записи в определенную позицию вектора анализируем какому блоку принадлежит 
         * данная позиция и, если не было сделано раньше, выделяем память под данный блок.
         * В конце концов получается, что память выделялась только под те блоки, к которым было обращение
         * И, как результат, экономия памяти
         * Vect - представляет собой массив блоков памяти. 
         * Max - Максимальное количество элементов в данном векторе
         * NBlocks - Количество блоков
         * BlockSize - Размер каждого блока (элементов)
         * MaxIndex - номер последнего ненулевого элемента
         * */
        int[][] Vect;
        int Max, NBlocks, BlockSize, MaxIndex=-1;

        public DinVect(int Max)
        {
            this.Max = Max;
            MaxIndex = -1;
            BlockSize = 16;
            NBlocks = Max / BlockSize + 1;      // Определяем, сколько надо блоков
            Vect = new int[NBlocks][];          // Определяем массив блоков
            // Но не выделяем память под сами блоки - будем делать это только по запросу в методе New()
        }

        // Метод, определяющий, в каком из блоков находится элемент с данной позицией
        // pos: Позиция элемента
        // i: Номер блока, в котором находится элемент
        // j: Позиция внутри блока
        public void GetIJ(int pos, out int i, out int j)
        {
            i = pos / BlockSize;        // Номер блока
            j = pos % BlockSize;        // Позиция внутри блока
        }

        // Метод, выделяющий память под блок
        // pos: Позиция, блок для которой нужно выделить
        public void New(int pos)
        {
            int i = 0, j = 0;
            GetIJ(pos, out i, out j);   // Определяем координаты данной позиции
            // Если память еще не выделена, то выделяем ее
            if (Vect[i] == null)
                Vect[i] = new int[BlockSize];
        }

        // Метод, возвращающий элемент по его номеру (позиции)
        // pos: Позиция элемента
        public int Get(int pos)
        {
            int i = 0, j = 0;
            if (pos < 0 || pos > Max)
                return -1;
            GetIJ(pos, out i, out j);   // Определяем координаты данной позиции
            try
            {
                return Vect[i][j];          // Возвращаем запрашиваемый элемент
            }
            catch
            {
                return -1;                   // Если же он не существует, то возвращаем ноль
            }
        }

        // Метод, с помощью которого заносится значение в вектор
        // pos: Позиция, в которую надо занести значение
        // value: Значение, которое надо занести
        // Возвращает отчет об успешности операции: true или false
        public bool Put(int pos, int value)
        {
            int i = 0, j = 0;
            // Если позиция за пределами вектора, то возвращаем значение, что операция не произведена
            if (pos < 0 || pos > Max)
                return false;
            GetIJ(pos, out i, out j);    // Определяем координаты данной позиции
            New(pos);                    // Выделяем память под блок
            Vect[i][j] = value;         // Заносим элемент
            if (pos > MaxIndex)
                MaxIndex = pos;
            return true;                // Возвращаем true, т.к. все выполнено успешно
        }

        // Метод, который освобождает память 
        public void Done()
        {
            for (int i = 0; i < NBlocks; i++)
                if (Vect[i] != null)
                    Vect[i] = null;
            Vect = null;
        }

        public int Length()
        {
            return MaxIndex + 1;
        }

        public void Print()
        {
            for (int i = 0; i <= MaxIndex; i++)
                Console.Write("{0} ", Get(i));
            Console.WriteLine();
        }
    }
}
