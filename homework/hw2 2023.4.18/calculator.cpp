# include"../../Stack/Stack.h"
# include<iostream>
using namespace std;

void trim ( const char exp[], int& lo, int& hi ){ //ɾ��exp[lo, hi]�������ŵ��ǰ׺����׺
	while ( ( lo <= hi ) && ( exp[lo] != '(' ) && ( exp[lo] != ')' ) ) lo++;//���ҵ�һ��
	while ( ( lo <= hi ) && ( exp[hi] != '(' ) && ( exp[hi] != ')' ) ) hi--; //���һ������
}

int divide ( const char exp[],int lo,int hi ){//�з�exp[lo��hi]��ʹexpƥ������ӱ��ʽƥ�� 
	int mi = lo; int crc = 1; //crcΪ[1o��mi]��Χ������������Ŀ֮��
	while( ( 0 < crc ) && ( ++mi < hi ) ) { //��������ַ���ֱ������������Ŀ��ȣ�����Խ��
		if( exp[mi] == '(' ) crc--; 
		if( exp[mi] == ')' ) crc++; }//�������ŷֱ����
    return mi; //��mi <= hi����Ϊ�Ϸ��зֵ�; ������ζ�žֲ�������ƥ��
}

bool paren( const char exp[],int lo,int hi){//�����ʽexp[lo��hi]�Ƿ�����ƥ��(�ݹ��)
	trim ( exp,lo, hi );if( lo > hi ) return true;//����������ŵ�ǰ׺����׺ 
	if( exp[lo] != '(') return false; //���ַ��������ţ���ز�ƥ��
	if( exp[hi] != ')') return false; //ĩ�ַ��������ţ���ز�ƥ��
	int mi = divide( exp, lo, hi ); //ȷ���ʵ����зֵ� 
	if( mi > hi ) return false;  //�зֵ㲻�Ϸ�����ζ�žֲ��������岻ƥ��
	return paren( exp, lo + 1, mi-1 ) && paren( exp, mi + 1, hi ); //�ֱ��������ӱ��ʽ
}

float readNumber(char* &S, Stack<float> &opnd) {
	float x = 0.0;
	bool point=false;//�ж��Ƿ�ΪС��
	int i = 0;//��С������λ��
	while (isdigit(*S)||*S=='.') {
		if (*S != '.') {
			if (point == false)	
				x = x * 10 + (*(S++) - 48);//��ȥ48,ASCIIת������
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
	opnd.push(x);//ѹ�������ջ
	return x;
}


#define N_OPTR 9 //���������
typedef enum { ADD, SUB, MUL, DIV, POW, FAC, L_P, R_P, EOE } Operator; //���������
//�ӣ������ˣ������˷����׳ˣ������ţ������ţ���ʼ������ֹ��

const char pri[N_OPTR][N_OPTR] = { //��������ȵȼ�[ջ��][��ǰ]
/*					|-----------------------------��ǰ����� ------------------------| */ 
/*					+		-		*		/		^		!		(		)		\0 */ 
/*  -- + */        '>',    '>',    '<',    '<',    '<',    '<',    '<',    '>',     '>', 
/*  |  - */ 	   '>',    '>',    '<',    '<',    '<' ,   '<',    '<',    '>',     '>', 
/* ջ  * */        '>',    '>',    '>',    '>',    '<',    '<',    '<',    '>',     '>', 
/* ��  / */        '>',    '>',    '>',    '>',    '<',    '<',    '<',    '>',     '>', 
/* ��  ^ */        '>',    '>',    '>',    '>',    '>',    '<',    '<',    '>',     '>',
/* ��  ! */        '>',    '>',    '>',    '>',    '>',    '>',    ' ',    '>',     '>',
/* ��  ( */	       '<',    '<',    '<',    '<',    '<',    '<',    '<',    '=',     ' ',
/*  |  ) */        ' ',    ' ',    ' ',    ' ',    ' ',    ' ',    ' ',    ' ',     ' ',
/* -- \0 */        '<',    '<',    '<',    '<',    '<',    '<',    '<',    ' ',     '=',
};


char optr2rank(char op)
{//�ɲ�����ת������
	switch(op)
	{
		case'+' : return ADD;//��
		case'-' : return SUB;//��
		case'*' : return MUL;//��
		case'/' : return DIV;//��
		case'^' : return POW;//�˷�
		case'!' : return FAC;//�׳�
		case'(' : return L_P;//������
		case')' : return R_P;//������
		case'\0' : return EOE;//��ʼ������ֹ��
		default : exit(-1);//λ�������
	}
}

char orderBetween(char op1, char op2) //�Ƚ�����������֮������ȼ�
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


float evaluate ( char* S){ //��(���޳��׿ո��)���ʽS��ֵ����ת��Ϊ�沨�����ʽRPN 
	Stack<float> opnd; Stack<char> optr; //������ջ�������ջ 
	optr.push( '\0' ); //β�ڱ�'\0'��Ϊͷ�ڱ�������ջ
	while ( !optr.empty() ) {  //������� 
		if( isdigit ( *S ) ) {
			readNumber ( S, opnd ); //��������������������RPNĩ
		}else //����ǰ�ַ�Ϊ���������
			switch ( orderBetween (optr.top(), *S ) ) {//������ջ�������֮�����ȼ�Ļ�ͷֱ��� 
				case'<': //ջ����������ȼ�����ʱ
					optr.push( *S ); S++ ;  //�����Ƴ٣���ǰ�������ջ
					break;
				case'=':  //���ȼ����(��ǰ�����Ϊ�����Ż���β���ڱ�'\0')ʱ 
					optr.pop(); S++; //�����Ų�������һ���ַ� 
					break;
				case'>' : {  //ջ����������ȼ�����ʱ����ʵʩ��Ӧ�ļ��㣬�������������ջ
					char op = optr.pop();//append ( RPN, op );  //ջ���������ջ��������RPNĩβ 
					if('!'== op ) {  //������һԪ�����
						float pOpnd = opnd.pop(); //ֻ��ȡ��һ������������
						opnd.push ( calcu ( op, pOpnd ) );//ʵʩһԪ���㣬�����ջ
					} else {  //��������(��Ԫ)�����
						float pOpnd2 = opnd.pop();
						float pOpnd1 = opnd.pop();//ȡ����ǰ������ 
						opnd.push ( calcu ( pOpnd1, op, pOpnd2 ) );  //ʵʩ��Ԫ���㣬�����ջ
					}
					break;
				}
				default : exit( -1 );  //���﷨���󣬲�������ֱ���˳�
			} //switch
	} //while
	return opnd.pop(); //�������������ļ�����
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
	if(!paren(G,0,20)) cout <<"��������,���Ų�ƥ��"<<endl; 
	
}  
