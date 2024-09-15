#ifndef PREDEFINE_H
#define PREDEFINE_H

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
    #pragma execution_character_set("utf-8")
#endif

/*
这部分代码使用了条件编译指令来根据编译器类型和版本号来决定是否执行某些代码。

#if defined(_MSC_VER) &amp;&amp; (_MSC_VER &gt;= 1600)：检查是否定义了_MSC_VER宏，并且其值是否大于或等于1600。
_MSC_VER是Microsoft Visual C++编译器定义的一个宏，表示编译器的版本号。1600对应的是Visual Studio 2010及以后的版本。
#pragma execution_character_set(&quot;utf-8&quot;)：这是一个编译器指令，用于指定执行字符集为UTF-8。
这个指令通常用于确保在编译过程中，源代码文件中的字符编码被正确处理。
文件结束
*/

#endif // PREDEFINE_H
