/*****************************************************************************
 *                                                                           *
 *    Card Sharks                                                      v1.1  *
 *        by                                                                 *
 *    Yannick Tessier                                                        *
 *                                                                           *
 *    v5 original: Yannick Tessier April 2, 1989                             *
 *    MBBS6/WG1/WG2 Port: Rick Hadsall June 28, 2004                         *
 *    WG3.2 port: Rick Hadsall February 2, 2006                              *
 *    V10/WG32 single kit: Rick Hadsall August 16, 2024                      *
 *                                                                           *
 * Copyright (C) 2024 Rick Hadsall.  All Rights Reserved.                    *
 *                                                                           *
 * This program is free software: you can redistribute it and/or modify      *
 * it under the terms of the GNU Affero General Public License as published  *
 * by the Free Software Foundation, either version 3 of the License, or      *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
 * GNU Affero General Public License for more details.                       *
 *                                                                           *
 * You should have received a copy of the GNU Affero General Public License  *
 * along with this program. If not, see <https://www.gnu.org/licenses/>.     *
 *                                                                           *
 * Additional Terms for Contributors:                                        *
 * 1. By contributing to this project, you agree to assign all right, title, *
 *    and interest, including all copyrights, in and to your contributions   *
 *    to Rick Hadsall and Elwynor Technologies.                              *
 * 2. You grant Rick Hadsall and Elwynor Technologies a non-exclusive,       *
 *    royalty-free, worldwide license to use, reproduce, prepare derivative  *
 *    works of, publicly display, publicly perform, sublicense, and          *
 *    distribute your contributions                                          *
 * 3. You represent that you have the legal right to make your contributions *
 *    and that the contributions do not infringe any third-party rights.     *
 * 4. Rick Hadsall and Elwynor Technologies are not obligated to incorporate *
 *    any contributions into the project.                                    *
 * 5. This project is licensed under the AGPL v3, and any derivative works   *
 *    must also be licensed under the AGPL v3.                               *
 * 6. If you create an entirely new project (a fork) based on this work, it  *
 *    must also be licensed under the AGPL v3, you assign all right, title,  *
 *    and interest, including all copyrights, in and to your contributions   *
 *    to Rick Hadsall and Elwynor Technologies, and you must include these   *
 *    additional terms in your project's LICENSE file(s).                    *
 *                                                                           *
 * By contributing to this project, you agree to these terms.                *
 *                                                                           *
 *****************************************************************************/
 
#include "gcomm.h"
#include "majorbbs.h"
#include "elwcards.h"

/* function declarations */
GBOOL cardsinp(VOID);
GBOOL cardslon(VOID);
VOID clscards(VOID);
INT gtcard(INT which);
INT rnd52(VOID);
VOID showlvl(VOID);
VOID wtocard(VOID);
VOID acolor(INT front,INT back,INT intense);

static
HMCVFILE csmb;                /* Skeleton named-message file block pointer */
FILE *tops;                   /* Top Ten Players                           */

static INT cardsstt;          /* Card Sharks State */
struct module elwcards={      /* module interface block               */
     "",                      /*    name used to refer to this module */
     cardslon,                /*    user logon supplemental routine   */
     cardsinp,                /*    input routine if selected         */
     NULL,                    /*    status-input routine if selected  */
     NULL,                    /*    "injoth" routine for this module  */
     NULL,                    /*    user logoff supplemental routine  */
     NULL,                    /*    hangup (lost carrier) routine     */
     NULL,                    /*    midnight cleanup routine          */
     NULL,                    /*    delete-account routine            */
     clscards                 /*    finish-up (sys shutdown) routine  */
};
 
struct tops {                 /*    Top Ten List structure              */
  CHAR name[30];              /*    Names of top ten finishers          */
  LONG score;                 /*    Their scores.                       */
} top[10];
 
//struct  tops top[10];

INT showlogon;

struct card {
  LONG money;
  INT  cards[28],
       value[28],
       which,
       level,
       type;
  LONG  bet;
   INT ansi;
 
} *card, *cds;
 
#define VERSION "1.1"

void EXPORT 
init__elwcards(void)
{
     INT ctr; INT fchar,findex,fscanf_return;

     stzcpy(elwcards.descrp, gmdnam("ELWCARDS.MDF"), MNMSIZ);
     cardsstt = register_module(&elwcards);
  
     tops=fopen("elwcards.dat","r");
     for (ctr=0;ctr < 10;ctr++) {     
     	findex=0;	
     	while(1) { fchar=fgetc(tops); if ((fchar != EOF) && (fchar != 13) && (fchar != 10)) top[ctr].name[findex++] = (CHAR)fchar; else break; }
     	top[ctr].name[findex] = '\0'; 
        fscanf_return=fscanf(tops,"%ld",&top[ctr].score);
        fgetc(tops);
        (VOID)fscanf_return;
     }
     fclose(tops);

     csmb=opnmsg("elwcards.mcv");
     setmbk(csmb);
     showlogon=ynopt(CARDSMSV);
 
#ifdef __BUILDV10MODULE
     card = (struct card*)alcmem((INT)nterms * sizeof(struct card));
#else
     card = (struct card*)getml((LONG)nterms * sizeof(struct card));
#endif

     if (card == NULL) {
          catastro("ELWCARDS: NOT ENOUGH MEMORY"); // honestly this was v5.x stuff, it shouldn't happen -RH
          return; // unreachable
     }
     for (usrnum=0 ; usrnum < nterms ; usrnum++) {
          setmem(&card[usrnum],sizeof(struct card),0);
     }

#ifdef __BUILDV10MODULE
     unsigned int seed;

#ifdef _WIN32
     FILETIME ft;
     GetSystemTimeAsFileTime(&ft);

     ULARGE_INTEGER ui = { .QuadPart = 0 };
     (VOID)ui;
     ui.LowPart = ft.dwLowDateTime;
     ui.HighPart = ft.dwHighDateTime;
     seed = (unsigned int)(ui.QuadPart ^ (unsigned int)GetCurrentThreadId());
#else
     // Linux or MacOS
     struct timeval tv;
     gettimeofday(&tv, NULL);
     seed = (unsigned int)(tv.tv_sec ^ tv.tv_usec);
#ifdef __MACH__
     // macOS seeding using mach_absolute_time
     seed ^= (unsigned int)mach_absolute_time();
#else
     // linux seeding using process ID
     seed ^= (unsigned int)getpid();
#endif // if __MACH__

#endif // if _WIN32

#else
     randomize(); // Borland C/C++ 5 proprietary randomizer
#endif  // if __BUILDV10MODULE
 
     shocst(spr("ELW Card Sharks v%s", VERSION), "(C) Copyright 2024 Elwynor Technologies - www.elwynor.com");
}

GBOOL cardslon(VOID) {
   if (showlogon) { 
       setmbk(csmb);
       prfmsg(CSLOGIN);
       outprf(usrnum);
   } 
   return(0);
}

GBOOL cardsinp(VOID)
{
  INT x,win,count;

      cds=&(card[usrnum]);
      setmbk(csmb);
      if (!hasmkey(CARDSKEY)) {
        prfmsg(NONMEM);
        return(0);
      }
      cds->ansi=(usaptr->ansifl%2);
      switch (usrptr->substt) {
        case 0:
          prfmsg(MAIN);
          usrptr->substt=1;
          cds->ansi=0;
          break;
        case 1:
          switch (tolower(*margv[0])) {
            case 'x':
              prfmsg(EXIT,usaptr->userid);
              return(0);
            /*case 'a':
              if (cds->ansi==0)
                cds->ansi=1;
              else
                cds->ansi=0;
              prfmsg(ANSI+cds->ansi);
              prfmsg(MAIN);
              break;*/
            case 'i':
              prfmsg(INSTRUCT);
              prfmsg(OPTION);
              break;
            case 'p':
              for (x=0;x<28;x++) {
                if (gtcard(x) == 1)
                  x--;
              }
              cds->type=0;
              if (usaptr->systyp > 1)
                cds->type=1;
              cds->which=0;
              cds->level=0;
              cds->money=250;
              showlvl();
              usrptr->substt=3;
              if (cds->which == cds->level) {
                usrptr->substt=2;
                prfmsg(CHANGE);
              }
              else
                prfmsg(BID,ltoa(cds->money));
              break;
            case 't':
              prfmsg(TOPTEN);
              for (count=0;count<10;count++) {
                prfmsg(TOPS,top[count].name,ltoa(top[count].score));
              }
              prfmsg(OPTION);
              break;
            default:
              prfmsg(MAIN);
              break;
          }
          break;
        case 2:
          if (sameas(margv[0],"N") || sameas(margv[0],"Y")) {
            if (sameas(margv[0],"N")) {
              cds->cards[cds->which]=cds->cards[cds->level+22];
              cds->value[cds->which]=cds->value[cds->level+22];
              showlvl();
            }
            usrptr->substt=3;
            prfmsg(BID,ltoa(cds->money));
            break;
          }
          if (sameas(margv[0],"X")) {
              prfmsg(MAIN);
              usrptr->substt=1;
              break;
          }
          showlvl();
          prfmsg(CHANGE);
          break;
        case 3:
          if (atol(margv[0]) > 0 && atol(margv[0]) <= cds->money) {
            if (cds->level == 4) {
              if (atol(margv[0]) < cds->money/2) {
                prfmsg(LESS);
                showlvl();
                prfmsg(BID,ltoa(cds->money));
                break;
              }
            }
            cds->bet=atol(margv[0]);
            prfmsg(HIGHER);
            usrptr->substt=4;
            break;
          }
          if (sameas(margv[0],"X")) {
              prfmsg(MAIN);
              usrptr->substt=1;
              break;
          }
          prfmsg(ONLYBID,ltoa(cds->money));
          showlvl();
          prfmsg(BID,ltoa(cds->money));
          break;
        case 4:
          win=0;
          if (sameas(margv[0],"x")) {
              prfmsg(MAIN);
              usrptr->substt=1;
              break;
          }
          if (sameto(margv[0],"lower")) {
            if (cds->value[cds->which] > cds->value[cds->which+1])
              win=1;
            if (cds->value[cds->which] < cds->value[cds->which+1])
              win=2;
            if (cds->value[cds->which] == cds->value[cds->which+1])
              win=3;
          }
          if (sameto(margv[0],"higher")) {
            if (cds->value[cds->which] < cds->value[cds->which+1])
              win=1;
            if (cds->value[cds->which] > cds->value[cds->which+1])
              win=2;
            if (cds->value[cds->which] == cds->value[cds->which+1])
              win=3;
          }
          if (win != 0) {
            cds->which++;
            usrptr->substt=3;
            if (win == 3)
              prfmsg(DRAW);
            if (win == 2) {
              prfmsg(LOSE);
              cds->money=cds->money-cds->bet;
              if (cds->money == 0) {
                showlvl();
                prfmsg(BROKE);
                prfmsg(MAIN);
                usrptr->substt=1;
                break;
              }
            }
            if (win == 1) {
              prfmsg(WIN);
              cds->money=cds->money+cds->bet;
            }
            if (cds->which == 21) {
              prfmsg(FINAL,ltoa(cds->money));
              win=11;
              for (count=9;count>=0;count--) {
                if (top[count].score < cds->money)
                  win=count;
              }
              if (win < 11) {
                prfmsg(WINNING,ltoa(win));
                for (count=9;count>=win; count--)  {
                  strcpy(top[count].name,top[count-1].name);
                  top[count].score= top[count-1].score;
                }
                top[win].score = cds->money;
                strcpy(top[win].name,usaptr->userid);
                wtocard();
              }
              prfmsg(MAIN);
              usrptr->substt=1;
              break;
            }
            if (cds->which == 4 || cds->which == 9 || cds->which == 14 ||
                cds->which == 19) {
                showlvl();
                cds->which++;
                cds->cards[cds->which]=cds->cards[cds->which-1];
                cds->value[cds->which]=cds->value[cds->which-1];
                cds->level++;
                cds->money=cds->money+250;
                prfmsg(EXTRA);
            }
            showlvl();
            if (cds->which  == cds->level*5) {
              prfmsg(CHANGE);
              usrptr->substt=2;
            }
            else
             prfmsg(BID,ltoa(cds->money));
          }
          else
            prfmsg(HIGHER);
          break;
      }
      outprf(usrnum);
      return(1);
}

INT gtcard(INT which)
{
 INT x,y;
 
 x=which;
 
 cds->cards[x]=rnd52();
 for (y=0;y<x;y++) {
   if (cds->cards[y]==cds->cards[x])
     return(1);
 }
 if (cds->cards[x] < 14)
   cds->value[x] = cds->cards[x];
 
 if (cds->cards[x] > 13 && cds->cards[x] < 27)
   cds->value[x] = cds->cards[x]-13;
 
 if (cds->cards[x] > 26 && cds->cards[x] < 40)
   cds->value[x] = cds->cards[x]-26;
 
 if (cds->cards[x] > 39)
   cds->value[x] = cds->cards[x]-39;
 
 if (cds->value[x] == 1)
   cds->value[x]=14;
 
 return(2);
}

// DOS - MBBS625/WG1/WG2 - rnd52() routine
/*INT rnd52(VOID)
{
  union REGS regs;
  INT numy = 3569;
 
  regs.h.ah=0x2c;
  int86(0x21,&regs,&regs);
  numy = numy + (numy*(regs.h.dh+1)*(regs.h.dl+1)) + regs.h.dl;
  return(abs(numy)%52+1);
}*/

// Return a random number from 1-52
INT rnd52(VOID) 
{ 
#ifdef __BUILDV10MODULE
    // Major BBS V10
    static INT numy = 3569;

    // Works on Windows, Linux, or MacOS
    time_t t = time(NULL);
    struct tm* tm_info = localtime(&t);
    numy = numy + (numy * (tm_info->tm_hour + 1) * (tm_info->tm_sec + 1)) + tm_info->tm_sec;

    return ((abs(numy) % 52) + 1);
#else
    // Borland C/C++ 5.0 for Win32
    return random(52)+1;
#endif
}

VOID showlvl(VOID) 
{
  INT x;
  INT kind;
  setmbk(csmb);
      btutsw(usrnum,0);
      if (cds->level < 4) {
        acolor(6,0,1);
        if (cds->type == 0)
          prfmsg(CARDT,ltoa(cds->level+1));
        else
        prfmsg(CARDTA,ltoa(cds->level+1));
        for (x=cds->level*5;x<cds->level*5+5;x++) {
          if (x <= cds->which) {
            prfmsg(CARDM+cds->type);
            kind=cds->cards[x]/13;
            if (kind==4)
              kind=3;
            acolor(kind+1,0,1);
            if (cds->value[x] > 10) {
              prfmsg(CARD1+cds->value[x]-11);
              prf(" ");
            }
            else
              prf(" %-2s",ltoa(cds->value[x]));
            prfmsg(KIND1+kind+cds->type*4);
            acolor(6,0,1);
            prfmsg(CARDM+cds->type);
          }
          else
            prfmsg(CARDBB+cds->type);
        }
        outprf(usrnum);
        clrprf();
        prfmsg(CARDMM+cds->type);
        for (x=cds->level*5;x<cds->level*5+5;x++) {
          if (x <= cds->which) {
            prfmsg(CARDM+cds->type);
            kind=cds->cards[x]/13;
            if (kind==4)
              kind=3;
            acolor(kind+1,0,1);
            prfmsg(KIND1+kind+cds->type*4);
            if (cds->value[x] > 10) {
              prf(" ");
              prfmsg(CARD1+cds->value[x]-11);
            }
            else
              prf(" %2s",ltoa(cds->value[x]));
            acolor(6,0,1);
            prfmsg(CARDM+cds->type);
          }
          else
            prfmsg(CARDBB+cds->type);
        }
        outprf(usrnum);
        clrprf();
        prf("\n");
        prfmsg(CARDB+cds->type);
      }
      else  {
        acolor(6,0,1);
        if (cds->type == 0)
          prfmsg(CARDT1,ltoa(cds->level+1));
        else
        prfmsg(CARDT1A,ltoa(cds->level+1));
        for (x=cds->level*5;x<cds->level*5+2;x++) {
          if (x <= cds->which) {
            prfmsg(CARDM+cds->type);
            kind=cds->cards[x]/13;
            if (kind==4)
              kind=3;
            acolor(kind+1,0,1);
            if (cds->value[x] > 10) {
              prfmsg(CARD1+cds->value[x]-11);
              prf(" ");
            }
            else
              prf(" %-2s",ltoa(cds->value[x]));
              prfmsg(KIND1+cds->type*4+kind);
            acolor(6,0,1);
            prfmsg(CARDM+cds->type);
          }
          else
            prfmsg(CARDBB+cds->type);
        }
        prfmsg(CARDMM1+cds->type);
        for (x=cds->level*5;x<cds->level*5+2;x++) {
          if (x <= cds->which) {
            prfmsg(CARDM+cds->type);
            kind=cds->cards[x]/13;
            if (kind==4)
              kind=3;
            acolor(kind+1,0,1);
            prfmsg(KIND1+cds->type*4+kind);
            if (cds->value[x] > 10) {
              prf(" ");
              prfmsg(CARD1+cds->value[x]-11);
            }
            else
              prf(" %2s",ltoa(cds->value[x]));
            acolor(6,0,1);
            prfmsg(CARDM+cds->type);
          }
          else
            prfmsg(CARDBB+cds->type);
        }
        prf("\n");
        prfmsg(CARDB1+cds->type);
      }
      btutsw(usrnum,usaptr->scnwid);
 
}

VOID wtocard(VOID)
{
  INT yg;
  tops=fopen("elwcards.dat","w");
  for ( yg=0;yg<9;yg++) {
    fprintf(tops,"%s\n",&top[yg].name);
    fprintf(tops,"%ld\n",top[yg].score);
  }
  fclose(tops);
}

VOID acolor(INT front,INT back,INT intense)
{
   if (front==0)
     front = 7;
     front=front+30;
     back=back+40;
     if (cds->ansi == 1)
       prf("[%d;%d;%dm",intense,front,back);
}

VOID clscards(VOID)
{
  clsmsg(csmb);	
}
