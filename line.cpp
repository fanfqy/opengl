#include<GL\glut.h>
#include <math.h>
#include<iostream>
using namespace std;
int X0 = 0;
int Y0 = 0;
int X1 = 0;
int Y1 = 0;
int drawmode = 0;
int Color = 0;
bool firstClick = true;
void drawpixel(int x, int y, int color)
{
    // 提取RGB分量
    int r = (color & 0xFF0000) >> 16;
    int g = (color & 0x00FF00) >> 8;
    int b = (color & 0x0000FF);

    // 转换为浮点数
    float rf = r / 255.0f;
    float gf = g / 255.0f;
    float bf = b / 255.0f;

    // 设置颜色
    glColor3f(rf, gf, bf);
    glBegin(GL_POINTS);
    glVertex2i(x, y);
    glEnd();
}
void swap(int x, int y)
{
    int a = 0;
    a = y;
    y = x;
    x = a;
}
// 初始化函数
void init(void) {
    glClearColor(1.0, 1.0, 1.0, 0.0); // 设置背景颜色为白色
    glMatrixMode(GL_PROJECTION);       // 设置投影模式
    gluOrtho2D(0.0, 400.0, 0.0, 300.0); // 设置视口大小
}
void DDALine(int x0, int y0, int x1, int y1, int color)
{
    int dx, dy, ep, i;  //ep 为max(|dx|,|dy|), i 为绘线控制参数
    float x, y, xIn, yIn; // x, y为计算得到的下一点值，xIn，
    // yIn为两个方向的增量
    dx = x1 - x0;
    dy = y1 - y0;
    x = x0;    //初始化至起点
    y = y0;
    if (abs(dx) > abs(dy))  ep = abs(dx);// 确定ep的取值
    else  ep = abs(dy);
    xIn = (float)dx / (float)ep;//计算两个方向的增量。实际应
    yIn = (float)dy / (float)ep;//用中，要注意避免除数为零 
    for (i = 0; i <= ep; i++) {
        drawpixel(int(x + 0.5), int(y + 0.5), color);
        x += xIn;
        y += yIn;
    }
}
void bresenham_line(int x0, int y0, int x1, int y1, int color)
{
    int dx = x1 - x0;
    int dy = y1 - y0;
    int stepX = dx >= 0 ? 1 : -1;
    int stepY = dy >= 0 ? 1 : -1;
    dx = abs(dx);
    dy = abs(dy);

    if (dx > dy) { // |m| < 1
        int p = 2 * dy - dx;
        int y = y0;
        for (int x = x0; x != x1; x += stepX) {
            drawpixel(x, y, color);
            if (p > 0) {
                y += stepY;
                p -= 2 * dx;
            }
            p += 2 * dy;
        }
    }
    else { // |m| >= 1
        int p = 2 * dx - dy;
        int x = x0;
        for (int y = y0; y != y1; y += stepY) {
            drawpixel(x, y, color);
            if (p > 0) {
                x += stepX;
                p -= 2 * dy;
            }
            p += 2 * dx;
        }
    }
}
void Midpoint_Line(int x0, int y0, int x1, int y1, int color)
{
    int a, b, d1, d2, d0, x, y;
    a = y0 - y1;              //初始化变量
    b = x1 - x0;
    d0 = 2 * a + b;
    d1 = 2 * a;
    d2 = 2 * (a + b);
    x = x0;
    y = y0;
    drawpixel(x, y, color);//画起点
    while (x < x1)              //根据是否到达终点进行循环判断
    {
        if (d0 < 0)             //中点在直线下方
        {
            x++;
            y++;
            d0 += d2;// d2=2* (a+b)
        }
        else                     //中点在直线上方
        {
            x++;
            d0 += d1;// d1=2*a
        }
        drawpixel(x, y, color);
    }  //  end while
} //  end MidPoint_Line()

void MidpointLine(int x0, int y0, int x1, int y1, int color) {
    int a, b, d1, d2, d, x, y;
    float m;
    if (x1 < x0) {
        // 交换起点和终点
        swap(x0, x1);
        swap(y0, y1);
    }

    a = y0 - y1;
    b = x1 - x0;
    if (b == 0) {
        m = -1 * a * 100;
    }//垂线处理
    else {
        m = (float)a / (x0 - x1);
    }
    x = x0;
    y = y0;
    drawpixel(x, y, color);
    if (m >= 0 && m <= 1) {
        d = 2 * a + b;
        d1 = 2 * a;
        d2 = 2 * (a + b);
        while (x < x1) {
            if (d <= 0) {
                x++;
                y++;
                d += d2;
            }
            else {
                x++;
                d += d1;
            }
            drawpixel(x, y, color);
        }
    }
    else if (m <= 0 && m >= -1) {
        d = 2 * a - b;
        d1 = 2 * a - 2 * b;
        d2 = 2 * a;
        while (x < x1) {
            if (d > 0) {
                x++;
                y--;
                d += d1;
            }
            else {
                x++;
                d += d2;
            }
            drawpixel(x, y, color);
        }
    }
    else if (m > 1) {
        d = a + 2 * b;
        d1 = 2 * (a + b);
        d2 = 2 * b;
        while (y < y1) {
            if (d > 0) {
                x++;
                y++;
                d += d1;
            }
            else {
                y++;
                d += d2;
            }
            drawpixel(x, y, color);
        }
    }
    else {
        d = a - 2 * b;
        d1 = -2 * b;
        d2 = 2 * (a - b);
        while (y > y1) {
            if (d <= 0) {
                x++;
                y--;
                d += d2;
            }
            else {
                y--;
                d += d1;
            }
            drawpixel(x, y, color);
        }
    }
}

// 显示回调函数
void display(void) {
    glClear(GL_COLOR_BUFFER_BIT);   // 清除颜色缓冲区

    // 绘制一条线
    //
    switch (drawmode) {
    case 0:
        DDALine(0, 100, 400, 200, Color);
        break;
    case 1:
        bresenham_line(0, 200, 400, 100, Color);//-1<k<0
        break;
    case 2:
        bresenham_line(200, 0, 200, 400, Color);//垂线
        break;
    case 3:
        bresenham_line(200, 300, 300, 0, Color);//k<-1
        break;
    case 4:
        bresenham_line(100, 0, 200, 300, Color);//k>1
        break;
    case 5:
        MidpointLine(0, 200, 400, 0, Color);
        break;
    case 6:
        MidpointLine(200, 0, 200, 400, Color);
        break;
    case 9:
        bresenham_line(X0, Y0, X1, Y1, Color);
        break;
    }
    //DDALine(100, 0, 400, 0, 0x000000);
    //bresenham_line(0, 0, 400, 200);
    //MidpointLine(200, 0, 200, 400, Color);
    //Midpoint_Line(0, 0, 400, 200, 0x000000);
    glFlush(); // 强制执行所有OpenGL命令
}

void handleKeypress(unsigned char key, int x, int y) {
    switch (key) {
    case 'a': // 如果用户按下'a'键，改变线段的颜色
        Color = 0xFF0000; // 红色
        break;
    case 's': // 如果用户按下's'键，改变线段的颜色
        Color = 0x00FF00; // 绿色
        break;
    case 'd': // 如果用户按下'd'键，改变线段的颜色
        Color = 0x0000FF; // 蓝色
        break;
    case '0': // 如果用户按下'0'键，采用DDA进行绘图
        drawmode = 0;
        break;
    case '1': // 如果用户按下'1'键，采用bresenham进行绘图
        drawmode = 1;
        break;
    case '2': // 如果用户按下'2'键，采用bresenham进行绘图
        drawmode = 2;
        break;
    case '3': // 如果用户按下'3'键，采用bresenham进行绘图
        drawmode = 3;
        break;
    case '4': // 如果用户按下'4'键，采用bresenham进行绘图
        drawmode = 4;
        break;
    case '5': // 如果用户按下'5'键，采用中点画线法进行绘图
        drawmode = 5;
        break;
    case '6': // 如果用户按下'6'键，采用中点画线法进行绘图
        drawmode = 6;
        break;
    case '9': // 如果用户按下'9'键，采用中点画线法进行绘图
        drawmode = 9;
        break;
    default: // 其他键，不做任何操作
        return;
    }

    // 通知OpenGL窗口内容需要更新
    glutPostRedisplay();
}
void handleMouseClick(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        if (firstClick) {
            // 如果是第一次点击，更新线段的起点
            X0 = x;
            Y0 = 300 - y;
            firstClick = false;
        }
        else {
            // 如果是第二次点击，更新线段的终点
            X1 = x;
            Y1 = 300 - y;
            firstClick = true; // 重置为true，等待下一次起点的输入
        }

        // 通知OpenGL窗口内容需要更新
        glutPostRedisplay();
    }
}
int main(int argc, char** argv) {
    glutInit(&argc, argv);                           // 初始化glut库
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);     // 设置显示模式
    glutInitWindowPosition(50, 100);                 // 设置窗口位置
    glutInitWindowSize(400, 300);                    // 设置窗口大小
    glutCreateWindow("Drawing Window");              // 创建窗口

    init();                                          // 调用初始化函数
    glutDisplayFunc(display);                        // 注册显示回调函数
    glutKeyboardFunc(handleKeypress);
    glutMouseFunc(handleMouseClick);
    X0 = 0;
    Y0 = 0;
    X1 = 400;
    Y1 = 300;
    drawmode = 0;
    Color = 0x000000;
    glutMainLoop();                                  // 进入主循环
    return 0;
}

