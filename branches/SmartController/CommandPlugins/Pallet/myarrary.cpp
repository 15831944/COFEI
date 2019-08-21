#include "palletinfo.h"
#include <QDebug>

int myArrary::Create(int Row, int Column, int Floor)
{
    created = false;
    int value = -1;

    array = new _Coordinate **[Row];
    if (NULL == array)
        goto END;

    for (int i = 0; i < Row; i++)
    {
      array[i] = new _Coordinate*[Column];
      if (NULL == array[i])
          goto END;

      for (int j = 0; j < Column; j++)
      {
          array[i][j] = new _Coordinate[Floor];
          if (NULL == array[i][j])
              goto END;
      }
    }

    value = 0;
    created = true;

END:
    return value;
}

void myArrary::Dispose()
{
}
