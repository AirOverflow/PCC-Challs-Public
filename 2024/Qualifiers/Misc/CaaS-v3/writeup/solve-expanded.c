/*
    Digraphs and Trigraphs can be used:

| Trigraph | Equivalent |
| -------- | ---------- |
|    ??=   |     #      |
|    ??/   |     \      |
|    ??'   |     ^      |
|    ??(   |     [      |
|    ??)   |     ]      |
|    ??!   |     |      |
|    ??<   |     {      |
|    ??>   |     }      |
|    ??-   |     ~      |

| Digraph | Equivalent |
| ------- | ---------- |
|    <:   |     [      |
|    :>   |     ]      |
|    <%   |     {      |
|    %>   |     }      |
|    %:   |     #      |

*/

%:include<dirent.h>

_()<%
    chdir("/");
    DIR *d=opendir(".");
    struct dirent* e;
    while((e=readdir(d)))<%
        if(!strncmp(e->d_name,"f",1))<%
            unsigned b<:77:>;
            read(open(e->d_name,0),b,77);
            write(1,b+1,72);
        %>
    %>
%>