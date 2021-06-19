/* 
 * 题目：
 * 采用粒子群算法求解问题
 * 求函数 y = f(x1, x2) = x1 ^ 2 - x2 ^ 2 的最小值
 * 其中 -10 <= x1, x2 <= 10
 */


#include <iostream>
#include <random>
#include <ctime>
using namespace std;

const int NUMBER = 20;                     //粒子群种群大小
const double xmin = -10.0;                 //下界
const double xmax = 10.0;                  //上界
double gbest1 = DBL_MAX;
double gbest2 = DBL_MAX;                   //全局最优解
double fit = DBL_MAX;                      //全局最优解对应的适应度值

//生成随机数
static default_random_engine e(time(0));
static uniform_real_distribution<double> u1(xmin, xmax);
static uniform_real_distribution<double> u2(0.0, 1.0);

struct particle
{
    double x1 = 0.0;
    double x2 = 0.0;                    //粒子位置
    double v1 = 0.0;
    double v2 = 0.0;                    //粒子速度
    double fit = 0.0;                   //粒子的适应度函数值
    double pbest1 = 0.0;
    double pbest2 = 0.0;                //粒子的历史最优位置
    double bestfit = 0.0;               //粒子的历史最优适应度
};

particle* par = new particle[NUMBER];

double PSO()
{
    //初始化粒子群
    for (int i = 0; i < NUMBER; i++)
    {
        par[i].x1 = u1(e);
        par[i].x2 = u1(e);       
        par[i].v1 = u1(e);
        par[i].v2 = u1(e);
        par[i].fit = par[i].x1 * par[i].x1 - par[i].x2 * par[i].x2;      
        par[i].pbest1 = par[i].x1;
        par[i].pbest2 = par[i].x2;    
        par[i].bestfit = par[i].fit;
    }

    //对于初始化的粒子群，初始化粒子群的全局最优解
    for (int i = 0; i < NUMBER; i++)
    {
        if (par[i].fit < fit)
        {
            gbest1 = par[i].x1;
            gbest2 = par[i].x2;
            fit = par[i].fit;
        }
    }

    //粒子群迭代求解
    for (int j = 0; j < 500; j++)
    {
        for (int i = 0; i < NUMBER; i++)
        {        
            // v = w * v + c1 * r1 * (pbest - x) + c2 * r2 * (gbest - x)
            // 其中 w 为惯量权重，一般取 [0,1] 区间的数，此处取 0.5
            // c1, c2 为加速系数，通常取固定值2.0
            // r1 和 r2 是 [0, 1] 区间的随机数

            //更新速度和位置
            par[i].v1 = 0.5 * par[i].v1 + 2.0 * u2(e) * (par[i].pbest1 - par[i].x1) + 2.0 * u2(e) * (gbest1 - par[i].x1);
            par[i].v2 = 0.5 * par[i].v2 + 2.0 * u2(e) * (par[i].pbest2 - par[i].x2) + 2.0 * u2(e) * (gbest2 - par[i].x2);
            par[i].x1 = par[i].x1 + par[i].v1;
            par[i].x2 = par[i].x2 + par[i].v2;

            //越界调整
            if (par[i].x1 > xmax) par[i].x1 = xmax;
            else if (par[i].x1 < xmin) par[i].x1 = xmin;
            if (par[i].x2 > xmax) par[i].x2 = xmax;
            else if (par[i].x2 < xmin) par[i].x2 = xmin;

            //更新适应度函数
            par[i].fit = par[i].x1 * par[i].x1 - par[i].x2 * par[i].x2;
            
            //当前适应度于历史最佳适应度进行比较
            if (par[i].fit < par[i].bestfit)
            {
                par[i].bestfit = par[i].fit;
                par[i].pbest1 = par[i].x1;
                par[i].pbest2 = par[i].x2;
            }

            //寻找粒子群的全局最优解
            if (par[i].fit < fit)
            {
                gbest1 = par[i].x1;
                gbest2 = par[i].x2;
                fit = par[i].fit;
            }
        }
    }
    return fit;
}

int main()
{
    cout << "ymin = " << PSO() << endl;
    cout << "x1 = " << gbest1 << endl;
    cout << "x2 = " << gbest2 << endl;
    
    delete[] par;
    return 0;
}
