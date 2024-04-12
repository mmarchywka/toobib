#ifndef MJM_MISC_PARSE_H__
#define MJM_MISC_PARSE_H__

#include "mjm_globals.h"
#include "mjm_thread_util.h"
// copied in hex output code dropped this carp 
//#include "mjm_color_table.h"
#include "mjm_read_buffer.h"

#include <map> 
#include <vector> 
#include <algorithm>
#include <map>
#include <set>
#include <cmath>
#include <string>
#include <fstream>
#include <signal.h>
#include <stdlib.h>
#include <stdint.h>

#include <string>
#include <codecvt> // for std::codecvt_utf8
#include <locale>  // for std::wstring_convert
#include <cstdio>

// Fri May 31 05:24:38 EDT 2019
// generated by /home/documents/cpp/scripts/cpputil -classhdr mjm_misc_parse   
// g++ -std=gnu++11 -DTEST_MJM_MISC_PARSE -I. -I../../mjm/hlib -I../../mjm/num  -gdwarf-3 -O0  -x c++ mjm_misc_parse.h  -lpthread -lreadline

//https://stackoverflow.com/questions/18534494/convert-from-utf-8-to-unicode-c

/*
std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> conv_utf8_utf32;


int main() {

    std::string utf8_bytes = "ú";
    // print the bytes in %hex format
    for (char byte: utf8_bytes) {
        printf("%%%2X", reinterpret_cast<unsigned char&>(byte));
    }   
    printf("\n");


    std::u32string unicode_codepoints = conv_utf8_utf32.from_bytes(utf8_bytes);

*/




//template <class Tr>
template <class Tr,class Ch=char, class Iv=unsigned int >
class mjm_misc_parse 
{
 typedef mjm_misc_parse Myt;
 typedef typename Tr::IdxTy IdxTy;
 typedef typename Tr::D D;
 typedef typename Tr::StrTy StrTy;
 typedef typename Tr::Ss Ss;
 typedef typename Tr::IsTy IsTy;
 typedef typename Tr::OsTy OsTy;
 typedef typename Tr::Ofs Ofs;
public:
//typedef char Ch;
//typedef unsigned int Iv;

typedef  std::vector<Iv> Ctab;
// BITMASK may not always fit in an enum... 
enum { SZC=sizeof(Ch), BITS=SZC*8, BITMASK=(1<<BITS)-1, SZTAB=sizeof(Iv), VBITS=8*SZTAB };
 int myatoi(const StrTy & s )  { return myatoi(s.c_str()); }
 int myatoi(const char * c)  { return ::strtol(c,0,0); }



// typedef typename Tr::MyBlock  MyBlock;
public:
mjm_misc_parse() {}
~mjm_misc_parse() {}

static const bool is(const Iv&  m1,const Iv&  m2) { return (m1&m2)!=0; } 
static const bool mask(const Ch c,const Iv&  m) { return (Lut(c)&m)!=0; } 
static const Iv lut(const Ch c) { return Lut(c); } 
static const Iv*  lut() { return Lut(); } 
static StrTy remove_mask( const StrTy& s, const Iv & mask,const IdxTy flags=0) 
//{ return remove_mask(s,mask,(Myt*)0);}
{Myt x;  return remove_mask(s,mask,flags,&x);}
static StrTy remove_run( const StrTy& s, const Iv & mask) 
{Myt x;  return remove_run(s,mask,&x);}
template <class Ty>
static StrTy remove_mask( const StrTy& s, const Iv & mask,const IdxTy flags,  Ty * clutp=(Myt*)0) 
{ return  RemoveMask(  s,  mask, clutp,flags); }
template <class Ty>
static StrTy remove_run( const StrTy& s, const Iv & mask, Ty * clutp) 
{ return  RemoveRun(  s,  mask, clutp); }

static StrTy word(const char *p, const IdxTy len, const IdxTy first)
{ Myt x; return word(p,len,first,&x); }
template <class Ty>
static StrTy word(const char *p, const IdxTy len, const IdxTy first, Ty * clutp)
{ return Word(p,len,first,*clutp); }
enum { LCB =(0), DIGITB=(1),
WHITEB=(2), CTRLB=(3), UCB=(4), PUNCB=(5),EOLB=(6),STARTB=(7),
STOPB=(8), OPERB=(9), UNUSEDB=10 } ; 
enum { CARD=(1<<BITS) } ; 
enum { BAD=~0,/* BITS=8*sizeof(Ch), */ LC =(1<<LCB), DIGIT=(1<<DIGITB),
WHITE=(1<<WHITEB), CTRL=(1<<CTRLB), UC=(1<<UCB), PUNC=(1<<PUNCB),EOL=(1<<EOLB),START=(1<<STARTB),
STOP=(1<<STOPB), OPER=(1<<OPERB),UNUSED=(1<<UNUSEDB)} ; 

#if 0 
enum { BAD=~0,/* BITS=8*sizeof(Ch), */ CARD=(1<<BITS),LC =(1<<0), DIGIT=(1<<1),
WHITE=(1<<2), CTRL=(1<<3), UC=(1<<4), PUNC=(1<<5),EOL=(1<<6),START=(1<<7),
STOP=(1<<8), OPER=(1<<9),UNUSED=(1<<10), UNUSEDB=10 } ; 
#endif 


// more terms than needed but overkill here is of no consequence. 
enum { ASCII=LC|DIGIT|WHITE|CTRL|UC|PUNC|EOL|START|STOP|OPER } ; 
enum { ALPHA=LC|UC } ; 
enum { ALPHANUM=LC|UC|DIGIT } ; 
enum { DARK=LC|DIGIT|UC|PUNC|START|STOP|OPER } ; 
enum { PRINT=LC|DIGIT|WHITE|UC|PUNC|EOL|START|STOP|OPER } ; 

bool is_uc(const Iv iv ) const { return is(iv,UC); }  
bool is_lc(const Iv iv ) const { return is(iv,LC); }  
bool is_uc(const Ch c  ) const { return mask(c,UC); }  
bool is_lc(const Ch c  ) const { return mask(c,LC); }  
bool is_digit(const Ch c  ) const { return mask(c,DIGIT); }  
bool is_alpha(const Iv iv ) const { return is(iv,ALPHA); }  
bool is_digit(const Iv iv ) const { return is(iv,DIGIT); }  
// zed length is false 
bool is_uc(const Ch * p) const 
{ while (*p) { if (!is_uc(*p)) return false; ++p;} return true; }
// zed length is false 
bool is_uint(const Ch * p) const 
{ while (*p) {/* MM_ERR(MMPR(*p)) */  if (!is_digit(*p)) return false; ++p;} return true; }
// no scientific notation of actual parsing. 
bool is_float(const Ch * p) const 
{ while (*p) { if (!is_digit(*p)&&(*p!='.')&&(*p!='-')&&(*p!='+')) return false; ++p;} return true; }
bool is_floate(const Ch * p) const 
{ while (*p) { /* MM_ERR(MMPR(*p)) */ if (!is_digit(*p)&&(*p!='E')&&(*p!='e')&&(*p!='.')&&(*p!='-')&&(*p!='+')) return false; ++p;} return true; }


//enum{ BAD=CharLUT::BAD};
#if 0
 grep enum /home/documents/cpp/mjm/hlib//./mjm_misc_parse.h | sed -e 's/ /\n/g' | grep = | sed -e 's/=.*//' | sort | uniq  | awk '{x=x", "$1"=CharLUT::"$1}END{print x; }'
, =CharLUT::, ALPHA=CharLUT::ALPHA, ALPHANUM=CharLUT::ALPHANUM, ASCII=CharLUT::ASCII, BAD=CharLUT::BAD, BITS=CharLUT::BITS, CARD=CharLUT::CARD, DIGIT=CharLUT::DIGIT, MAP_MU=CharLUT::MAP_MU, PRINT=CharLUT::PRINT, SZC=CharLUT::SZC, SZTAB=CharLUT::SZTAB, TFLOAT=CharLUT::TFLOAT, TINT=CharLUT::TINT, TPUNC=CharLUT::TPUNC, UNUSED=CharLUT::UNUSED, UNUSEDB=CharLUT::UNUSEDB, VBITS=CharLUT::VBITS

#endif
template <class Ty> 
static IdxTy utf8_to_unicode(Ty & c, const char * p, const IdxTy flags)
{
typedef std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> U8UniTy;
U8UniTy  conv_utf8_utf32;
    std::u32string unicode_codepoints = conv_utf8_utf32.from_bytes(StrTy(p));

MM_LOOP(ii,unicode_codepoints) c.push_back(*ii); 
return 0;
} 

bool printable(const Ch c) { return LutMask(c,PRINT); } 
bool digit(const Ch c) { return LutMask(c,DIGIT); } 
bool white(const Ch c) { return LutMask(c,WHITE); } 
bool alphanum(const Ch c) { return LutMask(c,ALPHANUM); } 
// add html and url? 
// ordered range of non-negs unbounded 
template<class Tv>
bool evaluate_range(Tv & selvec,const StrTy & fn,const IdxTy flags=0) { return EvaluateRange( selvec, fn,flags); } 

// map of chosen values 
template<class Tv>
void eval_int_set(Tv & selmap,const StrTy & fn,const int lo, const int hi ) 
{ EvalIntSet( selmap, fn,lo,hi); } 


StrTy dump_lut(const IdxTy flags) { return DumpLut(flags); } 

StrTy dump(const IdxTy flags=0) { return Dump(flags); }
static IdxTy bad() { return ~0; } 

protected: // private:
typedef typename mjm_thread_util<Tr>::mutex_vector MutexVector;

enum { MAP_MU=0 , MU_SZ};
mutable MutexVector m_mutex_vector;
void EnterSerial(const IdxTy i)const  {  m_mutex_vector.enter_serial(i ); }
void ExitSerial(const IdxTy i)const  {  m_mutex_vector.exit_serial(i ); }
//m_mutex_vector = MutexVector(MU_SZ);

StrTy DumpLut(const IdxTy flags) 
{
const Iv * p = lut();
return DumpLut(p,flags); 
}
StrTy DumpLut(const Iv * p, const IdxTy flags=0 ) 
{
if (flags==1) { return DumpMembers(); } 
Ss ss; 
//return DumpLut();
for(IdxTy i=0; i<CARD; ++i)
{
ss<<i<<" "<<Sets(p[i])<<" "<<Printable(i)<<CRLF;

} 
return ss.str();
} 
template <class Ty>
static StrTy RemoveMask( const StrTy& s, const Iv & _mask, Ty & clut, IdxTy flags)
{
const IdxTy len=s.length();
Iv mask=_mask;
Ch c[len+2];
IdxTy ptr=0;
//const IdxTy mask= ParseTable::EOL; //  m_clut
#define RMCASE(CI,EX, EY,CP) for(IdxTy i=0; i<len; ++i) { const Ch cp=s.c_str()[CI]; const Iv lu= (*clut).lut(cp); if ((lu&mask)==0) { c[CP+ptr]=cp; ++ptr; EX  }else { EY}  }

#if 0 
for(IdxTy i=0; i<len; ++i)
{
const Ch cp=s.c_str()[i];
const Iv lu= (*clut).lut(cp);
if ((lu&mask)==0) { c[ptr]=cp; ++ptr; }
}
#endif

// you could do a lot better, just wanted to see what this would do :) 
// for leading anyway, just avoid copy and move start of new string etc. 
// and combine maybe... 
switch (flags)
{
case 0: { RMCASE( i,,, ) ; break; }  // all 
case 1: { RMCASE( i, mask =0; ,, ) ; break; }  // leading 
//case 2: { RMCASE(len-1-i, mask =0; ,,len-1-i-i ) ; break; }  // trailing  
case 2: { 
if (len==0) return s; 
memcpy(c,s.c_str(),len); IdxTy i=len-1;
while (i!=~0U) { if (((*clut).lut((s.c_str())[i])&mask)==0) { ptr=i+1;  break;  }       --i;  }  
break; }  // trailing  
default: { MM_ERR(" unsupported RemoveMask type "<<MMPR(flags)) return s; } 
} // switch 

c[ptr]=0;
return StrTy(c);
}
#undef RMCASE

template <class Ty>
static StrTy RemoveRun( const StrTy& s, const Iv & mask, Ty & clut)
{
const IdxTy len=s.length();
Ch c[len+2];
IdxTy ptr=0;
IdxTy run=1;  // remove leading whatever too 
//const IdxTy mask= ParseTable::EOL; //  m_clut
for(IdxTy i=0; i<len; ++i)
{
const Ch cp=s.c_str()[i];
const Iv lu= (*clut).lut(cp);
if ((lu&mask)==0) {run=0;  c[ptr]=cp; ++ptr; }
else { if (run==0) {c[ptr]=cp; ++ptr; }      ++run;  } 
}
c[ptr]=0;
return StrTy(c);
}
template <class Ty>
static StrTy Word(const char *p, const IdxTy len, const IdxTy first, Ty & clut)

{
IdxTy ifirst=~0;
IdxTy ilast=~0;
IdxTy state=0;
for(IdxTy i=first; i<len; ++i)
{
const Ch c=p[i];
if (c==0) { ilast=i; break; }
Iv v=clut.lut(c);
//if (Mask(v,ParseTable::WHITE)) {
if (Mask(v,WHITE)) {
if (state==0) continue;
if (state==1) { ilast=i; break ; }
} //mask
else
{ if (state==0){  ifirst=i; state=1; } }
}// for 
if (ifirst==~0) return StrTy();
if (ilast==~0) ilast=len;

const IdxTy sz=ilast-ifirst;
Ch ca[sz+1];
memcpy(ca,p+ifirst,sz);
ca[sz]=0;
return StrTy(ca);
} // Word





template<class Tv>
bool EvaluateRange(Tv & selvec,const StrTy & fn, const IdxTy flags)
{
const bool clear_first=Bit(flags,0);
if (clear_first) selvec.clear();
const IdxTy sz=fn.length();
const char * px=fn.c_str();
char pa[sz+1];
memcpy(pa,px,sz+1);
char * p=&pa[0];
IdxTy start=0;
IdxTy prior=BAD;
bool r=false;
for(IdxTy i=0; i<=sz; ++i)
{
const char c=p[i];
if ((c==0)||(c==',')) {
p[i]=0;
IdxTy n=myatoi(p+start);
if (r) {
for(IdxTy j=prior; j<=n; ++j) selvec.push_back(j);
MM_ERR(" selecting "<<MMPR2(prior,n))
r=false;
}
else
{ selvec.push_back(n); MM_ERR(" selecting "<<MMPR(n)) }
start=i+1;
prior=n;
} // comma  
if (c=='-' )
{ p[i]=0; IdxTy n=myatoi(p+start); r=true; prior=n; start=i+1; } // dash
}
//const IdxTy szs=selvec.size();
//if (szs!=0){ cu.ptr(selvec.back()); }
return r;
} //EvaluateRange 
template<class Tv>
void EvalIntSet(Tv & selmap,const StrTy & fn,const int lowest=0, const int highest=0) // { EvaluateIntSet( selmap, fn); } 
{


} // EvalIntSet 


//enum { BAD=~0,BITS=8*sizeof(Ch), CARD=(1<<BITS),LC =(1<<0), DIGIT=(1<<1),
//WHITE=(1<<2), CTRL=(1<<3), UC=(1<<4), PUNC=(1<<5),EOL=(1<<6),START=(1<<7),
//STOP=(1<<8), OPER=(1<<9) } ; 
#define MAPSTMJM( L ) m[L]=QUOTE(L);
template <class Tm> static void BitNames( Tm & m)
{
//m[LC]="LC";
MAPSTMJM(LC)
MAPSTMJM(DIGIT)
MAPSTMJM(WHITE)
MAPSTMJM(CTRL)
MAPSTMJM(UC)
MAPSTMJM(PUNC)
MAPSTMJM(EOL)
MAPSTMJM(START)
MAPSTMJM(STOP)
MAPSTMJM(OPER)
}
// printable : LC|UC|DIGIT|WHITE
static void Sep( Ss & ss) { ss<<"|"; } 
// this is really MASK 
static bool Bit(const Iv f, const IdxTy b) { return ((f&(b))!=0); } 
static bool LutMask(const Ch c, const IdxTy b) { return ((lut(c)&(b))!=0); } 
static bool Mask(const Iv f, const IdxTy b) { return ((f&(b))!=0); } 
static StrTy Printable(const IdxTy f)
{
Ss ss;
ss<<(Ch)f;
return ss.str();

}
static StrTy Sets(const Iv f)
{
Ss ss;
std::map<IdxTy,StrTy> m;
BitNames(m);
IdxTy cnt=0;
MM_LOOP(ii,m)
{
if (Mask(f,(*ii).first)) { if (cnt) Sep(ss);  ss<<(*ii).second; ++cnt; } 
} // ii 
return ss.str();
}

StrTy DumpMembers()
{
const Iv * p = lut();
return DumpMembers(p);
}
//////////////////////////////////////////
// copied from mjm_color_table 
static const char  lut(const IdxTy p) // const
{
static char c[16];
static bool init=false;
if (!init)
{
for(IdxTy i=0; i<10; ++i) c[i]='0'+i;
for(IdxTy i=0; i<6; ++i) c[i+10]='A'+i;

}
return c[p];
}
static void myhex(Ss & ss, const char c)
{
ss<<lut((c>>4)&15);
ss<<lut(c&15);
}
// wth is this 255 times a float??}
StrTy myhex(const D & x) const
{
Ss ss;
const IdxTy n=IdxTy(x*255);
ss<<(lut((n>>4)&15));
ss<<(lut((n>>0)&15));
return ss.str();
}




/////////////////////////////////////////
StrTy DumpMembers(const Iv * p )
{
//typedef mjm_color_table Th;
typedef Myt  Th;
Ss ss;
ss<<"DumpMembers"<<CRLF;
std::map<IdxTy,StrTy> m;
BitNames(m);
MM_LOOP(ii,m)
{
Ss sp;
sp<<(*ii).second;
IdxTy cnt=1;
for(IdxTy f=0; f<CARD; ++f)
{
if (Mask(p[f],(*ii).first)) { if (cnt) Sep(sp);  Th::myhex(sp,f); ++cnt; } 
} // f 
ss<<sp.str();
ss<<CRLF;
} // ii 
return ss.str();
}




static StrTy SetsOld(const Iv f)
{
Ss ss;
IdxTy cnt=0; 
if (Bit(f,DIGIT)){ if (cnt) Sep(ss);  ss<<"DIGIT"; ++cnt; } 
if (Bit(f,LC)){ if (cnt) Sep(ss);  ss<<"LC"; ++cnt; } 
if (Bit(f,WHITE)){ if (cnt) Sep(ss);  ss<<"WHITE"; ++cnt; } 
if (Bit(f,CTRL)){ if (cnt) Sep(ss);  ss<<"CTRL"; ++cnt; } 
if (Bit(f,UC)){ if (cnt) Sep(ss);  ss<<"UC"; ++cnt; } 
if (Bit(f,PUNC)){ if (cnt) Sep(ss);  ss<<"PUNC"; ++cnt; } 
if (Bit(f,EOL)){ if (cnt) Sep(ss);  ss<<"EOL"; ++cnt; } 
if (Bit(f,START)){ if (cnt) Sep(ss);  ss<<"START"; ++cnt; } 
if (Bit(f,STOP)){ if (cnt) Sep(ss);  ss<<"STOP"; ++cnt; } 
if (Bit(f,OPER)){ if (cnt) Sep(ss);  ss<<"OPER"; ++cnt; } 
return ss.str();
}



static void Set(Iv * p, const IdxTy o, const IdxTy f, const Iv v)
{ for(IdxTy i=o; i<=f; ++i) p[i]|=v; }
static void Set(Iv * p, const Iv m, const Ch * s)
{ const Ch * q=s; while (*q!=0) { p[IdxTy(*q)]|=m; ++q; }  }

static void SetN(Iv * p, const Iv m,const IdxTy n, const Ch * s)
{ 
const Ch * q=s; 
const Ch * qend=q+n; 
while (q!=qend) { p[IdxTy(*q)]|=m; ++q; }  }


static void Reset(Iv * p, const Iv m, const Ch * s)
{ const Ch * q=s; while (*q!=0) { p[IdxTy(*q)]&=m; ++q; }  }

static void ResetN(Iv * p, const Iv m,const IdxTy n, const Ch * s)
{ 
const Ch * q=s; 
const Ch * qend=q+n; 
while (q!=qend) { p[IdxTy(*q)]&=m; ++q; }  }

static void Flip(Iv * p, const Iv m, const Ch * s)
{ const Ch * q=s; while (*q!=0) { p[IdxTy(*q)]^=m; ++q; }  }

static void FlipN(Iv * p, const Iv m,const IdxTy n, const Ch * s)
{ 
const Ch * q=s; 
const Ch * qend=q+n; 
while (q!=qend) { p[IdxTy(*q)]^=m; ++q; }  }


static Ctab MakeCtab()
{

Ctab x(CARD);
//memset(&x[0],0,CARD*sizeof(IdxTy));
memset(&x[0],0,CARD*SZTAB);
Set(&x[0],0,0x01f,CTRL);
Set(&x[0],0x041,0x05A,UC);
Set(&x[0],0x061,0x07A,LC);
Set(&x[0],0x030,0x039,DIGIT);
Set(&x[0],WHITE," \t\r\n");
Set(&x[0],START,"\"'<[{(");
Set(&x[0],STOP,"\"'>]})");
Set(&x[0],OPER,"!$%^&*-+/<>=?|");
//x[(int)' ']|=WHITE; x[(int)'\t']|=WHITE; 
x[(int)'\r']|=WHITE|EOL;
x[(int)'\n']|=WHITE|EOL;
Set(&x[0],0x021,0x02f,PUNC);
Set(&x[0],0x03A,0x040,PUNC);
Set(&x[0],0x05b,0x060,PUNC);
Set(&x[0],0x07b,0x07f,PUNC);
// added later, need to think
x[0]|=EOL;
return x;
}


static int ChSignFix(const Ch c) { return (int(c)&BITMASK); }

static const Iv Lut(const Ch c)
{ // this should sign extend doh
//return Lut()[int(c)];
return Lut()[ChSignFix(c)];

}
static const Iv* Lut()
{
return &LutVec()[0];
}
static const Ctab &  LutVec()
{
static Ctab ctab= MakeCtab();
//return ctab[c];
//return &ctab[0];
return ctab;
}
StrTy Dump(const IdxTy flags=0) {Ss ss; ss<<" this does nothing now "<<CRLF;   return ss.str(); }


}; // mjm_misc_parse

//template <class Tr> 
template <class Tr,class Ch=char, class _Iv=unsigned int >
class mjm_var_parse : public mjm_misc_parse<Tr,Ch,_Iv>
{
protected:
typedef mjm_misc_parse<Tr,Ch,_Iv> Super;
typedef typename Super::Ctab Ctab;

 typedef typename Tr::IdxTy IdxTy;
 typedef typename Tr::D D;
 typedef typename Tr::StrTy StrTy;
 typedef typename Tr::Ss Ss;
 typedef typename Tr::IsTy IsTy;
 typedef typename Tr::OsTy OsTy;
 typedef typename Tr::Ofs Ofs;
public: 
//typedef typename Super::Iv Iv;
//typedef typename Super::Ch Ch;
typedef _Iv Iv;
typedef std::map<IdxTy, StrTy> Ubits;
//typedef char Ch;
//typedef unsigned int Iv;
enum { VBITS=Super::VBITS, UNUSED=Super::UNUSED, UNUSEDB=Super::UNUSEDB };




public:
mjm_var_parse() { Init(); } 


const bool mask(const Ch c,const Iv&  m) const  { return (Lut(c)&m)!=0; } 
const Iv lut(const Ch c) const  { return Lut(c); } 
const Iv*  lut() { return Lut(); } 

IdxTy ubit(const StrTy&  nm) { return NextBit(nm); }
IdxTy ubit() {  return NextBit(""); }
StrTy dump_lut() { return DumpLut(); } 

// add mask m to every element in s 
 void set( const Iv m, const Ch * s) { Super::Set(Lut(),  m,  s); } 
 void reset( const Iv m, const Ch * s) { Super::Reset(Lut(),  m,  s); } 
 void flip( const Iv m, const Ch * s) { Super::Flip(Lut(),  m,  s); } 

void set( const Iv m, const IdxTy n, const Ch * s) 
{ Super::SetN(Lut(),  m, n, s); } 
void reset( const Iv m, const IdxTy n, const Ch * s) 
{ Super::Reset(Lut(),  m,n,  s); } 
void flip( const Iv m, const IdxTy n, const Ch * s) 
{ Super::Flip(Lut(),  m, n, s); } 

// make a new user bit and set that for s 
 Iv uset(  const Ch * s) { 
const IdxTy b=ubit();
const Iv m=(1<<b);
Super::Set(Lut(),  m,  s); 
return m; 
} 
//typedef mjm_read_buffer<Tr,Ch> RdBuff;
typedef mjm_read_buffer<> read_buffer;
// user needs to clear or whatever before calling 
enum { TINT='i', TFLOAT='f',TALPHA='a', TPUNC='p',TWHITE='w',TUNK='u'};
void parse_groups(read_buffer & rb, const Ch * s)
{
if (s==0) return; 
const Ch * p=s;
Ch ty;
Ch* pty=&ty;
while (*p!=0)
{
int i=0;
const Ch c=p[i];
const Iv cc=lut(c);
// number, could also start - or . 
if (mask(c,Super::DIGIT)||(c=='-')||(c=='.')) 
{
bool have_neg=(p[i]=='-');
bool have_dec=(p[i]=='.');
bool have_digit=!have_neg&&!have_dec;
++i;
while(mask(p[i],Super::DIGIT)||(p[i]=='.')) 
{
if (p[i]!='.')  have_digit=true;
else {  if (have_dec) break; have_dec=true; } 
// if - or ., could still be a punctuation 
// exclude exponents 
++i;
} // while 
if (have_digit)  ty=have_dec?TFLOAT:TINT;
else ty=Super::PUNC;
}// DIGIT etc
else if (mask(c,Super::WHITE)) 
{ty=TWHITE; while (mask(p[i],Super::WHITE))  ++i; }
else if (mask(c,Super::ALPHA)) 
{ ty=TALPHA;  while (mask(p[i],Super::ALPHA))  ++i; }
else {ty=TUNK; while (Tunk(p[i])) { ++i; }  }
// push the type code 
rb.push_string(pty,1);
const IdxTy sz=i;
// push the actual string 
rb.push_string(p,sz);
p+=sz;
} // while

} // parrse_groups
bool Tunk(const Ch c )
{
if (c==0) return false; // end the think 
const Iv m=Super::DIGIT| Super::WHITE|Super::ALPHA;
bool maybe= (mask(c,m)||(c=='-')||(c=='.')) ;
//MM_ERR(MMPR3(c,mask(c,m),maybe))
return !maybe;
} 

private:

void Init()
{
m_ctab=Super::LutVec();
m_first_ubit=UNUSEDB;
m_next_ubit=UNUSEDB;
}
IdxTy NextBit(const StrTy nm ) 
{ IdxTy b=(++m_next_ubit); 
if (b>=VBITS) { MM_ERR(" too many user bits "<<MMPR3(nm,b,m_first_ubit)) }  
if (b>= VBITS) b=Super::bad();
if (b>=VBITS) 
m_map[b]=nm; 
return b; 
}
StrTy DumpLut() { return Super::DumpLut(Lut()); }
// this should sign extend??? 
 Iv Lut(const Ch c)const  { return Lut()[int(c)]; }
 Iv* Lut() { return &LutVec()[0]; }
 const Iv* Lut()const  { return &LutVec()[0]; }
Ctab &  LutVec() { return m_ctab; }
const Ctab &  LutVec()const  { return m_ctab; }

Ctab m_ctab;
Ubits m_map;
IdxTy m_first_ubit,m_next_ubit;

};  // mjm_misc_parse





//////////////////////////////////////////////

template <class Tr>
class mjm_misc_parse_map : public std::map<typename Tr::StrTy, mjm_misc_parse< Tr > >  
{
 typedef mjm_misc_parse_map Myt;
typedef typename std::map<typename Tr::StrTy, mjm_misc_parse< Tr> >   Super;
 typedef typename Tr::IdxTy IdxTy;
 typedef typename Tr::D D;
 typedef typename Tr::StrTy StrTy;
 typedef typename Tr::Ss Ss;
 typedef typename Tr::IsTy IsTy;
 typedef typename Tr::OsTy OsTy;
 typedef typename Tr::Ofs Ofs;
// typedef typename Tr::MyBlock  MyBlock;
public:
mjm_misc_parse_map() {}
typedef typename mjm_thread_util<Tr>::mutex_vector MutexVector;

enum { MAP_MU=0 , MU_SZ};
mutable MutexVector m_mutex_vector;
void EnterSerial(const IdxTy i)const  {  m_mutex_vector.enter_serial(i ); }
void ExitSerial(const IdxTy i)const  {  m_mutex_vector.exit_serial(i ); }
//m_mutex_vector = MutexVector(MU_SZ);


StrTy dump(const IdxTy flags=0) { return Dump(flags); }

private:

void Init()
{
}

StrTy Dump(const IdxTy flags=0)
{
Ss ss;
MM_LOOP(ii,(*this))
{
ss<<(*ii).first<<CRLF;
ss<<(*ii).second.dump()<<CRLF;


}
return ss.str();
// return Dump(flags); 

}




private:

}; // mjm_misc_parse_map




////////////////////////////////////////////
#ifdef  TEST_MJM_MISC_PARSE
class Tr {
public:
// typedef mjm_string_picker Myt;
 typedef unsigned int IdxTy;
 typedef double  D;
 typedef std::string StrTy;
 typedef std::stringstream Ss;
 typedef std::istream  IsTy;
 typedef std::ostream  OsTy;
 typedef std::ofstream  Ofs;
// typedef typename Tr::MyBlock  MyBlock;
}; // 


#include "mjm_instruments.h"
#include "mjm_cli_ui.h"
typedef Tr::StrTy StrTy;
typedef Tr::IdxTy IdxTy;

class tester {
typedef mjm_cli_ui<tester> Cli;
public:
 void cli_cmd( Cli::list_type & choices,  const char * frag)
{
/*const IdxTy nfrag=strlen(frag);
MM_LOOP(ii,m_cmd_map)
{
const StrTy & v=(*ii).first;
if (strncmp(v.c_str(),frag,nfrag)==0)  choices.push_back(v);
*/
}

 void cli_param( Cli::list_type & choices,  const char * _cmd, const char * frag)
{
MM_ERR("cli_param"<<MMPR2(_cmd,frag))
//const StrTy cmd=CliTy::word(StrTy(_cmd),0);
//auto ii=m_comp_map.find(cmd);
//if ( ii!=m_comp_map.end()) ((this)->*(*ii).second)(choices,cmd.c_str(),frag);
}

 }; // tester
typedef mjm_cli_ui<tester> Cli;



int main(int argc,char **args)
{
MM_MSG(__FILE__<<__DATE__<<" "<<__TIME__)


typedef mjm_misc_parse<Tr>  Myt;
//Myt x(argc,args);
Myt x;

//if (!x.done()) x.command_mode();
Cli cli;
tester tester;
CommandInterpretter li(&std::cin);
li.push(args,argc);
cli.set_target(tester);
cli.set_command_handler(&tester::cli_cmd);
cli.set_param_handler(&tester::cli_param);
cli.activate();
li.set_split(1,' ');
while (li.nextok())
{
const IdxTy sz=li.size();
if (sz<1) continue;
const StrTy cmd=li.word(0);
if (cmd=="") continue;
CommandInterpretterParam  cip(li);
if (cmd=="quit") break;
//if (cmd=="dump") { MM_ERR(x.dump()) }
if (cmd=="dump") { MM_ERR(x.dump_lut(1)) }
if (cmd=="dumplut") { MM_ERR(x.dump_lut(0)) }
if (cmd=="ctab") {const char c=cip.p1.c_str()[0]; IdxTy v=x.lut(c); MM_ERR(std::hex<<MMPR2(((int)c),v)) }
//else if (cmd=="load") { x.load(li.words(),1); }
//else if (cmd=="clear") { x.clear(); }

} // nextok

//if (!x.done()) x.command_mode();
return 0;
}

#endif // main

#endif // MJM_MISC_PARSE_H__ 
