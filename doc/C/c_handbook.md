## 结构体声明
区别以下代码：
```c 
    struct {
        int     a;
        char    b;
        float   c;
    } y[20], *z;

    typedef struct {
        int     a;
        char    b;
        float   c;
    } Simple;
```
第一段声明创建了y和z。y是一个数组，它包含了20个结构，z是一个指针，它指向了这个类型的结构。

第二段它使用了typedef创建了一种新的类型，现在的Simple是一个类型名而不是结构标签，所以后续的声明可能像下面这个样子：
```c 
    Simple  x;
    Simple  y[20], *z;
```

## 结构体的自应用
如何在一个结构体内部包含一个类型为该结构本身的成员？
```c
    struct SELF_REF1{
        int     a;
        struct SELF_REF1 b;
        int     c;
    }
```
上面这个自引用是非法的，因为b是另一个完整的结构体，其内部还将包含自己的成员b，这样重复下去永无止境。而下面的示例是合法的：
```c 
    struct SELF_REF2{
        int     a;
        struct SELF_REF2 *b;
        int     c;
    }
```
同样在使用typedef技巧时，警惕以下陷阱：
```c 
    typedef struct {
        int     a;
        SELF_REF3 *b;
        int     c;
    } SELF_REF3;
```
原因是在使用SELF_REF3时，它还没被定义出来。
解决方案时定义一个结构标签来声明b，如下所示：
```c 
    typedef struct SELF_REF_TAG {
        int     a;
        struct SELF_REF_TAG *b;
        int     c;
    } SELF_REF3;
```
## 不完整的声明
在结构体相互引用时，可以使用不完整声明要为结构体标识“占个坑”,从而避免先有鸡还是先有蛋的难题。
```c
    struct B;

    struct A {
        struct B    *partner;
        /* other declaration */
    };

    struct B {
        struct A    *partner;
        /* other declaration */
    };
```

## 结构体的初始化
结构体的初始化类似与数组的初始化，综合使用花括号，逗号可以为复杂结构的结构完成初始化工作。
```c 
    struct INIT_EX {
        int     a;
        short   b[10];
        Simple  c;      //Simple has been defined above, 
                        //including a int, a char and a float
    } x = {
        10,
        {1,2,3,4,5},
        {25, 'x', 1.9}
    };
```

## 使用动态内存分配
这里有一个例子，他使用malloc分配一块内存
```c 
    int *pi;
    ...
    pi = malloc(100);
    if(pi == NULL){
        printf("Out of memory");
        exit(1);
    }
```
在内存分配成功的情况下，我们获得了一个指向100字节的指针。在整形为4个字节的机器上，这块内存被当作25个整型元素的数组，因为pi是一个指向整型的指针。

为了可移植性，方便在不同的机器上也能获得正确的结果，可以使用下面的技巧
```c 
    pi = malloc(25 * sizeof( int ));
```
通过下面的方式初始化数组元素：
```c 
    int *pi2, i;
    ...
    pi2 = pi;
    for(i = 0; i < 25; i++){
        *pi2++ = 0;
    }
```
也可以通过下标完成相同的工作：
```c
    int i;
    ...
    for(i = 0; i<25; i++){
        pi[i] = 0;
    }
```
## 字符串操作
```c 
    size_t  strlen( char const *string);
    char    *strcpy(char *dst, char const *src);
    char    *strcat(char *dst, char const *src);
    int     strcmp(char const *s1, char const *s2);
    char    *strncpy(char *dst, char const *src, size_t len);
    char    *strncat(char *dst, char const *src, size_t len);
    int     strncmp(char *dst, char const *s2, size_t len);
    char    *strchr(char const *str, int ch);
    char    *strrchr(char const *str, int ch);
    char    *strpbrk(char const *str, char const *group);
    char    *strstr(char const *s1, char const *s2);
    size_t  strspn(char const *str, char const *group);
    size_t  strcspn(char const *str, char const *group);

    int     iscntrl(int ch);
    int     isspace(int ch);
    int     isdigit(int ch);
    int     isxdigit(int ch);
    int     islower(int ch);
    int     isupper(int ch);
    int     isalpha(int ch);
    int     isalnum(int ch);
    int     ispunct(int ch);
    int     isgraph(int ch);
    int     isprint(int ch);

    int     tolower(int ch);
    int     toupper(int ch);

    void    *memcpy(void *dst, void const *src, size_t length);
    void    *memmove(void *dst, void const *src, size_t length);
    void    *memcmp(void const *a, void const *b, size_t length);
    void    *memchr(void const *a, int ch, size_t length);
    void    *memset(void *a, int ch, size_t length);
```