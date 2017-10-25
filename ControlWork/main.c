#include <stdio.h>
#include <stdlib.h>

int main()
{
    const int S = 2;
    int n, i, pos = S, it = -1;
    printf("%s\n", "Please, give me numbers of inquiries");
    scanf("%d", &n);
    printf("%s\n", "Inquiries are characterized by numbers 1 and 0. 1 means that the AddToEnd () function. 0 means At () function. Enter type of function and number though a gap");

    int** values;

    for (i = 0; i < n; i++)
    {
        int type, u;
        scanf("%d%d", &type, &u);
        if (type)
            printf("%s%d\n", "Your number is: ", values[u / S][u - u / S]);
        else
        {
            if (pos == S)
            {
                (int**)realloc(values, sizeof(int));
                it++, pos = 0;
                values[it] = (int*) malloc(sizeof(int) * S);
            }
            printf("%s", "Y!");
            values[it][pos] = u;
            pos++;
        }
    }

    return 0;
}
