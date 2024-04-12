#ifndef MJM_DIR_WATCH_H__
#define MJM_DIR_WATCH_H__

#include "mjm_globals.h"
#include "mjm_thread_util.h"
#include "mjm_strings.h"

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
#include <sys/inotify.h>
#include <errno.h>


// Mon Jul 11 15:34:14 EDT 2022
// generated by /home/documents/cpp/scripts/cpputil -classhdr mjm_dir_watch   
// g++  -Wall -std=gnu++11 -DTEST_MJM_DIR_WATCH -I. -I../../mjm/hlib -I../../mjm/num  -gdwarf-3 -O0  -x c++ mjm_dir_watch.h  -o mjm_dir_watch.out -lpthread -lreadline

mjm_global_credits::credit __credit__mjm_dir_watch("mjm_dir_watch"
, "  ");

template <class Tr>
class mjm_dir_watch 
{
 typedef mjm_dir_watch Myt;
 typedef typename Tr::IdxTy IdxTy;
 typedef typename Tr::D D;
 typedef typename Tr::StrTy StrTy;
 typedef typename Tr::Ss Ss;
 typedef typename Tr::IsTy IsTy;
 typedef typename Tr::OsTy OsTy;
 typedef typename Tr::Ofs Ofs;
//typedef typename Tr::FlagTy; 
// typedef typename Tr::MyBlock  MyBlock;
typedef int Watcher;
enum { BAD=~0 };



//ss<<MMPR4(ie->wd,ie->mask,ie->cookie,ie->len);
//const E*  e=(struct inotify_event*)(buf+off);
//StrTy fick_name=e->name;
//MM_ERR(MMPR(fick_name))
class _oo_event
{
public:
_oo_event(const struct inotify_event * ie)
{
m_wd=ie->wd; m_mask=ie->mask;
m_cookie=ie->cookie; m_len=ie->len;
m_name=ie->name;
}
StrTy dump() const
{
Ss ss;
ss<<MMPR4(m_wd,m_mask,m_cookie,m_len);
ss<<MMPR4(m_name,close(),create(),moved());
return ss.str();
}
const StrTy & name() const { return m_name; } 
bool close() const 
{ return 0!=(m_mask&(IN_CLOSE_WRITE|IN_CLOSE_NOWRITE)); } 
bool create() const { return 0!=(m_mask&(IN_CREATE)); } 
bool moved() const { return 0!=(m_mask&(IN_MOVED_TO|IN_MOVED_FROM)); } 
//m_mask=IN_CLOSE_WRITE|IN_CLOSE_NOWRITE|IN_CREATE|IN_MOVED_TO;
int m_wd, m_mask, m_cookie, m_len;
StrTy m_name;

}; // _oo_event

typedef _oo_event Event;

// TYPEDEFS

typedef std::vector<Event> Events;
typedef mjm_strings StrUtil;
// API

public:
typedef Event event_type;
mjm_dir_watch() {Init();}
~mjm_dir_watch() {}
IdxTy size() const { return m_events.size(); }
const event_type & last() const { return m_events.back(); } 
const event_type & event(const IdxTy i) const { return m_events[i]; } 
IdxTy watch(const StrTy & path, const IdxTy flags) { return Watch(path,flags); } 
IdxTy check(const IdxTy flags) { return Check(flags); } 
StrTy dump(const IdxTy flags=0) { return Dump(flags); }
private:
bool Bit(const IdxTy f, const IdxTy b) const  { return  ((f>>b)&1)!=0; }
// should loop over map now 
StrTy Dump(const IdxTy flags=0) {Ss ss;  
ss<<MMPR3(m_mask,m_watcher,m_path);
MM_LOOP(ii,m_wds) { ss<<" m_wds"<<MMPR((*ii)); } 
return ss.str(); }
typedef typename mjm_thread_util<Tr>::mutex_vector MutexVector;

enum { MAP_MU=0 , MU_SZ};
mutable MutexVector m_mutex_vector;
void EnterSerial(const IdxTy i)const  {  m_mutex_vector.enter_serial(i ); }
void ExitSerial(const IdxTy i)const  {  m_mutex_vector.exit_serial(i ); }
//m_mutex_vector = MutexVector(MU_SZ);
IdxTy Check( const IdxTy flags) 
{
MM_ERRF(MMPR(dump()))
// 2023-09-29 new fing chrome fck 
const bool discard_crdownload=!Bit(flags,0);
const IdxTy count= sizeof(struct inotify_event) + NAME_MAX + 1+(1<<10);
//(1<<12);
char buf[count];
ssize_t n=read(m_watcher,buf,count);
MM_ERR(MMPR2(__FUNCTION__,n))
if (n==0) return 0; 
if (n==BAD) { 

const IdxTy err=errno;
MM_ERR(" eagain ?"<<MMPR2(err,EAGAIN))
if (err==EAGAIN ) return 0; 
MM_ERR(MMPR(readec())) return n;  } 
typedef inotify_event E;
int off=0;
while (true)
{
MM_ERR(MMPR(off))
// ref should work as sizeof should not matter tight?
// references doesn't  work??? should just point
// to same thing not alloc new memr
// name get wtf
const E*  e=(struct inotify_event*)(buf+off);
bool skip_push=false;
if (discard_crdownload)
{
const StrTy s=".crdownload";
const char * p=e->name;
if (p) {
const IdxTy slen=strlen(p);
if (slen>s.length())
{

if (strcmp(s.c_str(),p+slen-s.length())==0) skip_push=true;
MM_ERR(MMPR3(s.c_str(),(p+slen-s.length()),skip_push)) ;
} // length 
} // p 
const bool end1=StrUtil::ends_in(p,".crdownload");
const bool end2=StrUtil::ends_in(p,".crdownload.html");
skip_push|=end1|end2;
MM_ERR(MMPR4(p,end1,end2,skip_push))

} // discard_crdownload 

//StrTy fick_name=e->name;
//MM_ERR(MMPR(fick_name))
//MM_ERR(MMPR(dump(e)))
event_type et=event_type(e);
m_events.push_back(et);
off+=e->len+ sizeof(inotify_event);
if (off>=n) break;

}
return 0;
}// Check
StrTy dump(const struct inotify_event*  ie) const
{
Ss ss;
ss<<MMPR4(ie->wd,ie->mask,ie->cookie,ie->len);
ss<<MMPR(&ie->name[0]);
const char * p=ie->name;
Ss fick;
for(IdxTy i=0; i<ie->len; ++i)
{
//ss<<" "<<MMPR3(i,(255 & (int(p[i]))),char(p[i]));
fick<<" "<<(255&(int(p[i])));
} // i 
ss<<fick.str();
return ss.str();
}  // dump
IdxTy Watch(const StrTy & path, const IdxTy flags) 
{ 
MM_ERR(MMPR3(__FUNCTION__,path,flags))

m_watcher=inotify_init1(IN_NONBLOCK);
WatcherOk(m_watcher);
m_path=path;
int rc=inotify_add_watch(m_watcher,path.c_str(),m_mask);
MM_ERR(MMPR3(__FUNCTION__,m_watcher,rc))
m_wds.push_back(rc);
return 0;  
} // Watch

IdxTy WatcherOk(const Watcher w)
{
if (w!=BAD) return 0;
const IdxTy err=errno;
switch (err)
{
case EINVAL :{ MM_ERR("EINVAL invalide value ") break; }  
case EMFILE :{ MM_ERR("EMFILE roo many  ") break; }  
//case EMFIL//E :{ MM_ERR("EINVAL invalide value ") break; }  
case ENFILE :{ MM_ERR("ENFILE too many system  ") break; }  
case ENOMEM :{ MM_ERR("ENOMEM not enough kernel memory ") break; }  

default: 
{ MM_ERR("unnown error  "<<MMPR(err)) break; }  

} // switch 
return BAD;
} // WatcherOk
#if 0 
#define EPERM        1  /* Operation not permitted */
#define ENOENT       2  /* No such file or directory */
#define ESRCH        3  /* No such process */
#define EINTR        4  /* Interrupted system call */
#define EIO      5  /* I/O error */
#define ENXIO        6  /* No such device or address */
#define E2BIG        7  /* Argument list too long */
#define ENOEXEC      8  /* Exec format error */
#define EBADF        9  /* Bad file number */
#define ECHILD      10  /* No child processes */
#define EAGAIN      11  /* Try again */
#define ENOMEM      12  /* Out of memory */
#define EACCES      13  /* Permission denied */
#define EFAULT      14  /* Bad address */
#define ENOTBLK     15  /* Block device required */
#define EBUSY       16  /* Device or resource busy */
#define EEXIST      17  /* File exists */
#define EXDEV       18  /* Cross-device link */
#define ENODEV      19  /* No such device */
#define ENOTDIR     20  /* Not a directory */
#define EISDIR      21  /* Is a directory */
#define EINVAL      22  /* Invalid argument */
#define ENFILE      23  /* File table overflow */
#define EMFILE      24  /* Too many open files */
#define ENOTTY      25  /* Not a typewriter */
#define ETXTBSY     26  /* Text file busy */
#define EFBIG       27  /* File too large */
#define ENOSPC      28  /* No space left on device */
#define ESPIPE      29  /* Illegal seek */
#define EROFS       30  /* Read-only file system */
#define EMLINK      31  /* Too many links */
#define EPIPE       32  /* Broken pipe */
#define EDOM        33  /* Math argument out of domain of func */
#define ERANGE      34  /* Math result not representable */


#endif



StrTy readec() const
{
const IdxTy err=errno;
Ss ss;
#define CRAP(x) case x : { ss<<QUOTE(x); break; } 
switch (err)
{
case 0: { ss<<" zero error "; break; } 
CRAP(EINVAL)
CRAP(EIO)
CRAP(EISDIR)
default:
ss<<" unknown erro "<<MMPR(err); 

} // err

return ss.str();
} // readed 
void Init()
{
m_watcher=BAD;
m_mask=IN_CLOSE_WRITE|IN_CLOSE_NOWRITE|IN_CREATE|IN_MOVED_TO;
}

// MEMBERS
uint32_t m_mask;
Watcher m_watcher;
StrTy m_path;
typedef std::vector<int> WatchDescs;
WatchDescs m_wds;
Events m_events;
}; // mjm_dir_watch

//////////////////////////////////////////////

template <class Tr>
class mjm_dir_watch_map : public std::map<typename Tr::StrTy, mjm_dir_watch< Tr > >  
{
 typedef mjm_dir_watch_map Myt;
typedef typename std::map<typename Tr::StrTy, mjm_dir_watch< Tr> >   Super;
 typedef typename Tr::IdxTy IdxTy;
 typedef typename Tr::D D;
 typedef typename Tr::StrTy StrTy;
 typedef typename Tr::Ss Ss;
 typedef typename Tr::IsTy IsTy;
 typedef typename Tr::OsTy OsTy;
 typedef typename Tr::Ofs Ofs;
//typedef typename Tr::FlagTy; 
// typedef typename Tr::MyBlock  MyBlock;
public:
mjm_dir_watch_map() {}
StrTy dump(const IdxTy flags=0) { return Dump(flags); }
private:
bool Bit(const IdxTy f, const IdxTy b) const  { return  ((f>>b)&1)!=0; }
// should loop over map now 
//StrTy Dump(const IdxTy flags=0) {Ss ss;  return ss.str(); }
typedef typename mjm_thread_util<Tr>::mutex_vector MutexVector;

enum { MAP_MU=0 , MU_SZ};
mutable MutexVector m_mutex_vector;
void EnterSerial(const IdxTy i)const  {  m_mutex_vector.enter_serial(i ); }
void ExitSerial(const IdxTy i)const  {  m_mutex_vector.exit_serial(i ); }
//m_mutex_vector = MutexVector(MU_SZ);


//StrTy dump(const IdxTy flags=0) { return Dump(flags); }

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

}; // mjm_dir_watch_map




////////////////////////////////////////////
#ifdef  TEST_MJM_DIR_WATCH
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

template <class Tt> class tester_ {
typedef tester_<Tt> Myt;
typedef mjm_cli_ui<Myt> Cli;
//typedef tester Myt;
//typedef mjm_cli_ui<Myt> Cli;
typedef std::map<StrTy, StrTy> LocalVar;

typedef CommandInterpretterParam Cip ;
typedef void (Myt:: * CmdFunc)(Cip &, LocalVar &  ) ;
typedef std::map<StrTy, CmdFunc> CmdMap;
typedef std::vector<StrTy> Choices;
//typedef void (Myt:: * CompleteFunc) ( Cli::list_type & choices,  const char * cmd, const char * frag);
typedef void (Myt:: * CompleteFunc) ( Choices & choices,  const char * cmd, const char * frag);
typedef std::map<StrTy, CompleteFunc> CompMap;

public:
 //void cli_cmd( Cli::list_type & choices,  const char * frag)
 void cli_cmd( Choices & choices,  const char * frag)
{
const IdxTy nfrag=strlen(frag);
MM_LOOP(ii,m_cmd_map)
{
const StrTy & v=(*ii).first;
if (strncmp(v.c_str(),frag,nfrag)==0)  choices.push_back(v);
}
}

 //void cli_param( Cli::list_type & choices,  const char * _cmd, const char * frag)
 void cli_param( Choices & choices,  const char * _cmd, const char * frag)
{
MM_ERR("cli_param"<<MMPR2(_cmd,frag))
//const StrTy cmd=CliTy::word(StrTy(_cmd),0);
//auto ii=m_comp_map.find(cmd);
//if ( ii!=m_comp_map.end()) ((this)->*(*ii).second)(choices,cmd.c_str(),frag);
}

CmdMap m_cmd_map;


 }; // tester_
typedef tester_< mjm_dir_watch <Tr>  > tester;

typedef mjm_cli_ui<tester> Cli;


typedef Tr::Ss Ss;
void about()
{
Ss ss;
ss<<" MJM_DIR_WATCH "<<__DATE__<<" "<<__TIME__<<CRLF;
MM_ERR(ss.str())

}

int main(int argc,char **args)
{
about();
typedef mjm_dir_watch<Tr>  Myt;
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
if (cmd=="about"){ about();  continue; } 
CommandInterpretterParam  cip(li);
const StrTy & p1=cip.p1;
const StrTy & p2=cip.p2;
if (cmd=="quit") break;
if (cmd=="watch") { MM_ERR(x.watch(p1,atoi(p2.c_str()))) }
if (cmd=="check") { MM_ERR(x.check(atoi(p1.c_str()))) }
if (cmd=="dump") { MM_ERR(x.dump()) }
//else if (cmd=="load") { x.load(li.words(),1); }
//else if (cmd=="clear") { x.clear(); }

} // nextok

//if (!x.done()) x.command_mode();
return 0;
}

#endif // main

#endif // MJM_DIR_WATCH_H__ 
