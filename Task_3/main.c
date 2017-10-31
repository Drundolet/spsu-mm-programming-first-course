#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>

void corner(double a, double b, double c)
{
    double t = acos((b * b + c * c - a * a) / (2 * b * c)) * 180 / M_PI;
    double x, y, z;
    modf(modf(modf(t, &x) * 100, &y) * 100, &z);
    printf("%d %d' %d\"\n", (int)round(x), (int)round(y), (int)round(z));
}

void foo(double a, double b, double c)
{
    if (a + b > c && a + c > b && b + c > a)
    {
        corner(a, b, c);
        corner(b, a, c);
        corner(c, a, b);
    }
    else
        printf("%s", "The triangle is degenerate");
}

const int MAX = 100;

bool out()
{
    printf("Symbol which was entered isn't admissible or the entered symbols aren't numbers.\nPlease repeat the attempt\n");
    return 0;
}

bool check(char a[MAX])
{
    int len = strlen(a) - 1;
    bool space = 0, point = 0;
    int i = 0;
    while (a[i] == ' ')
        i++;
    if (a[len - 1] == '.')
        return out();
    for (i; i < len; i++)
    {
        if ((a[i] < '0' || a[i] > '9') && a[i] != '.' && a[i] != ' ')
            return out();
        if (space && a[i] == '.')
            return out();
        if (point && a[i] == ' ')
            return out();
        if (a[i] == ' ')
            space = 1, point = 0;
        else
            if (a[i] == '.')
                point = 1, space = 0;
            else
                point = space = 0;
    }
    return 1;
}

void convert(char a[MAX], float b[3])
{
    int it;
    int i = 0, len = strlen(a) - 1;
    for (it = 0; it < 3; it++)
    {
        b[it] = 0;
        int point = -1;
        while (a[i] == ' ')
            i++;
        int j = i;
        while (a[j] != ' ' && j != len)
        {
            if (a[j] == '.')
                point = j;
            j++;
        }
        int t;
        float mn = 1;
        if (point == -1)
        {
            for (t = j - 1; t >= i; t--)
                b[it] += mn * (a[t] - '0'), mn *= 10;
        }
        else
        {
            mn = 1;
            for (t = point - 1; t >= i; t--)
                b[it] += (a[t] - '0') * mn, mn *= 10;
            mn = 10;
            for (t = point + 1; t < j; t++)
                b[it] += (a[t] - '0') / mn, mn *= 10;
        }
        i = j;
    }

}

int main()
{
    printf("%s", "Enter three numbers through a gap. Only numbers, a point and a gap are allowed.\n");
    char a[MAX];
    fgets(a, MAX, stdin);
    while (!check(a))
        fgets(a, MAX, stdin);
    float b[3];
    convert(a, b);
    foo(b[0], b[1], b[2]);
    return 0;
}
