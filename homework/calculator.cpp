# include"../../Stack/Stack.h"
# include<iostream>
using namespace std;

void trim ( const char exp[], int& lo, int& hi ){ //删除exp[lo, hi]不含括号的最长前缀、后缀
	while ( ( lo <= hi ) && ( exp[lo] != '(' ) && ( exp[lo] != ')' ) ) lo++;//查找第一个
	while ( ( lo <= hi ) && ( exp[hi] != '(' ) && ( exp[hi] != ')' ) ) hi--; //最后一个括号
}

int divide ( const char exp[],int lo,int hi ){//切分exp[lo，hi]，使exp匹配仅当子表达式匹配 
	int mi = lo; int crc = 1; //crc为[1o，mi]范围内左、右括号数目之差
	while( ( 0 < crc ) && ( ++mi < hi ) ) { //逐个检查各字符，直到左、右括号数目相等，或者越界
		if( exp[mi] == '(' ) crc--; 
		if( exp[mi] == ')' ) crc++; }//左、右括号分别计数
    return mi; //若mi <= hi，则为合法切分点; 否则，意味着局部不可能匹配
}

bool paren( const char exp[],int lo,int hi){//检查表达式exp[lo，hi]是否括号匹配(递归版)
	trim ( exp,lo, hi );if( lo > hi ) return true;//清除不含括号的前缀、后缀 
	if( exp[lo] != '(') return false; //首字符非左括号，则必不匹配
	if( exp[hi] != ')') return false; //末字符非右括号，则必不匹配
	int mi = divide( exp, lo, hi ); //确定适当的切分点 
	if( mi > hi ) return false;  //切分点不合法，意味着局部以至整体不匹配
	return paren( exp, lo + 1, mi-1 ) && paren( exp, mi + 1, hi ); //分别检查左、右子表达式
}

float readNumber(char* &S, Stack<float> &opnd) {
	float x = 0.0;
	bool point=false;//判断是否为小数
	int i = 0;//计小数点后的位数
	while (isdigit(*S)||*S=='.') {
		if (*S != '.') {
			if (point == false)	
				x = x * 10 + (*(S++) - 48);//减去48,ASCII转成数字
			if (point == true) 
			{ 	x = x * 10 + (*(S++) - 48);
				i++; }
		}
		else { S++; point = true; }
	}
	int temp = 1;
	for (int j = 0; j < i; j++) 
	temp *= 10;
	x = x / temp;
	opnd.push(x);//压入操作数栈
	return x;
}


#define N_OPTR 9 //运算符总数
typedef enum { ADD, SUB, MUL, DIV, POW, FAC, L_P, R_P, EOE } Operator; //运算符集合
//加，减，乘，除，乘方，阶乘，左括号，右括号，起始符与终止符

const char pri[N_OPTR][N_OPTR] = { //运算符优先等级[栈顶][当前]
/*					|-----------------------------当前运算符 ------------------------| */ 
/*					+		-		*		/		^		!		(		)		\0 */ 
/*  -- + */        '>',    '>',    '<',    '<',    '<',    '<',    '<',    '>',     '>', 
/*  |  - */ 	   '>',    '>',    '<',    '<',    '<' ,   '<',    '<',    '>',     '>', 
/* 栈  * */        '>',    '>',    '>',    '>',    '<',    '<',    '<',    '>',     '>', 
/* 顶  / */        '>',    '>',    '>',    '>',    '<',    '<',    '<',    '>',     '>', 
/* 运  ^ */        '>',    '>',    '>',    '>',    '>',    '<',    '<',    '>',     '>',
/* 算  ! */        '>',    '>',    '>',    '>',    '>',    '>',    ' ',    '>',     '>',
/* 符  ( */	       '<',    '<',    '<',    '<',    '<',    '<',    '<',    '=',     ' ',
/*  |  ) */        ' ',    ' ',    ' ',    ' ',    ' ',    ' ',    ' ',    ' ',     ' ',
/* -- \0 */        '<',    '<',    '<',    '<',    '<',    '<',    '<',    ' ',     '=',
};


char optr2rank(char op)
{//由操作符转译出编号
	switch(op)
	{
		case'+' : return ADD;//加
		case'-' : return SUB;//减
		case'*' : return MUL;//乘
		case'/' : return DIV;//除
		case'^' : return POW;//乘方
		case'!' : return FAC;//阶乘
		case'(' : return L_P;//左括号
		case')' : return R_P;//右括号
		case'\0' : return EOE;//起始符与终止符
		default : exit(-1);//位置运算符
	}
}

char orderBetween(char op1, char op2) //比较两个操作符之间的优先级
{
	return pri[optr2rank(op1)][optr2rank(op2)];
}

int jiecheng( int pOpnd ){
	int sum = 1;
	for(int i = 2; i <= pOpnd; i++)
		sum *= i;
	return sum; 
}

int calcu ( char op, int pOpnd ){
	int l = jiecheng(pOpnd);
	
}

float calcu ( float pOpnd1, char op, float pOpnd2 ){
	float tot = 1;
	switch (op)
	{
		case '+': {tot = pOpnd1 + pOpnd2; break;}
		case '-': {tot = pOpnd1 - pOpnd2; break;}
		case '*': {tot = pOpnd1 * pOpnd2; break;}
		case '/': {tot = pOpnd1 / pOpnd2; break;}
		case '^': {for(int i = 0; i < pOpnd1; i++) {tot *= pOpnd2;} break;}
	}
	return tot;
	}


float evaluate ( char* S){ //对(已剔除白空格的)表达式S求值，并转换为逆波兰表达式RPN 
	Stack<float> opnd; Stack<char> optr; //运算数栈、运算符栈 
	optr.push( '\0' ); //尾哨兵'\0'作为头哨兵首先入栈
	while ( !optr.empty() ) {  //逐个处理 
		if( isdigit ( *S ) ) {
			readNumber ( S, opnd ); //读入操作数，并将其接至RPN末
		}else //若当前字符为运算符，则
			switch ( orderBetween (optr.top(), *S ) ) {//视其与栈顶运算符之间优先级幕低分别处理 
				case'<': //栈顶运算符优先级更低时
					optr.push( *S ); S++ ;  //计算推迟，当前运算符进栈
					break;
				case'=':  //优先级相等(当前运算符为右括号或者尾部哨兵'\0')时 
					optr.pop(); S++; //脱括号并接收下一个字符 
					break;
				case'>' : {  //栈顶运算符优先级更高时，可实施相应的计算，并将结果重新入栈
					char op = optr.pop();//append ( RPN, op );  //栈顶运算符出栈井续接至RPN末尾 
					if('!'== op ) {  //若属于一元运算符
						float pOpnd = opnd.pop(); //只需取出一个操作数，并
						opnd.push ( calcu ( op, pOpnd ) );//实施一元计算，结果入栈
					} else {  //对于其它(二元)运算符
						float pOpnd2 = opnd.pop();
						float pOpnd1 = opnd.pop();//取出后、前操作数 
						opnd.push ( calcu ( pOpnd1, op, pOpnd2 ) );  //实施二元计算，结果入栈
					}
					break;
				}
				default : exit( -1 );  //逢语法错误，不做处理直接退出
			} //switch
	} //while
	return opnd.pop(); //弹出并返回最后的计算结果
}
int main(){
	char A[] = "3.4+2.5"; 
	cout << "3.4+2.5 = "<<evaluate(A)<<endl;
	char B[] = "3-2";
	cout << "3-2 = "<<evaluate(B)<<endl;
	char C[] = "7.3*8.9";
	cout << "7.3*8.9 = "<<evaluate(C)<<endl;
	char D[] = "30/2";
	cout << "30/2 = "<<evaluate(D)<<endl;
	char E[] = "5.5-4!+20.34";
	cout <<"5.5-4!+20.34 = "<<evaluate(E)<<endl;
	char F[] = "(5-4)!+20";
	cout <<"(5-4)!+20 = "<<evaluate(F)<<endl;
	char G[] = "(2+3)*(4-1)/3+(4-2)!)";
	if(!paren(G,0,20)) cout <<"输入有误,括号不匹配"<<endl; 
	
}  
