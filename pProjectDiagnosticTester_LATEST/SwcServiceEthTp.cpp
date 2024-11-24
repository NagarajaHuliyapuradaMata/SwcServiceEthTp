/******************************************************************************/
/* File   : SwcServiceEthTp.cpp                                               */
/* Author : Nagaraja HULIYAPURADA-MATA                                        */
/* Date   : 01.02.1982                                                        */
/******************************************************************************/

/******************************************************************************/
/* #INCLUDES                                                                  */
/******************************************************************************/
#include "TypesStd.hpp"

#include "infClientSwcServiceEthTp.hpp"
#include "interface_Persistency.hpp"

#include <iostream>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>

/******************************************************************************/
/* #DEFINES                                                                   */
/******************************************************************************/

/******************************************************************************/
/* MACROS                                                                     */
/******************************************************************************/

/******************************************************************************/
/* TYPEDEFS                                                                   */
/******************************************************************************/
typedef struct{
   uint32                   u32Domain;
   uint32                   u32SocketType;
   uint32                   u32Protocol;
}Type_CfgMcalEth_stSocket;

typedef struct{
   Type_CfgMcalEth_stSocket stSocket;
   socklen_t                tSizeAddress;
   struct sockaddr_in       stAddress;
   uint32                   u32FlagsSend;
}Type_CfgMcalEth;

typedef struct{
   sint32 s32Socket;
}Type_McalEth_stChannel;

class Type_SwcServiceEthTp:
      public infClientSwcServiceEthTp
{
   private:
      Type_McalEth_stChannel stChannel;
      Type_CfgMcalEth        stCfg;

   public:
      void vInitFunction   (void);
      void vRead           (      sint8* ps8Buffer,  uint32 u32LengthBuffer);
      void vWrite          (const sint8* pcs8Buffer, uint32 u32LengthBuffer);
      void vMainFunction   (void);
      void vDeInitFunction (void);
};

/******************************************************************************/
/* CONSTS                                                                     */
/******************************************************************************/

/******************************************************************************/
/* PARAMS                                                                     */
/******************************************************************************/

/******************************************************************************/
/* OBJECTS                                                                    */
/******************************************************************************/
Type_SwcServiceEthTp            SwcServiceEthTp;
infClientSwcServiceEthTp* const cpstinfClientSwcServiceEthTp = &SwcServiceEthTp;

/******************************************************************************/
/* FUNCTIONS                                                                  */
/******************************************************************************/
void Type_SwcServiceEthTp::vInitFunction(void){
   pPersistency->vGetCfg(
         (uint64*)&this->stCfg
      ,  "/home/nagarajahuliyapuradamata/Workspace_Ubuntu/repo_projects/pProjectDiagnosticTester/main_LATEST/source/SwcServiceEthTp/CfgMcalEth.hex"
   );

   if(
         0
      >  (
            this->stChannel.s32Socket = socket(
                  this->stCfg.stSocket.u32Domain
               ,  this->stCfg.stSocket.u32SocketType
               ,  this->stCfg.stSocket.u32Protocol
            )
         )
   ){
      std::cerr << std::endl << "ERROR: socket" << std::endl;
      exit(EXIT_FAILURE);
   }

   if(
         0
      >  connect(
                                   this->stChannel.s32Socket
            ,  (struct sockaddr*) &this->stCfg.stAddress
            ,  (socklen_t)         this->stCfg.tSizeAddress
         )
   ){
      std::cerr << std::endl << "ERROR: connect" << std::endl;
      exit(EXIT_FAILURE);
   }
}

void Type_SwcServiceEthTp::vDeInitFunction(void){
   close(this->stChannel.s32Socket);
}

void Type_SwcServiceEthTp::vRead(
      sint8* ps8Buffer
   ,  uint32 u32LengthBuffer
){
   read(
         this->stChannel.s32Socket
      ,  ps8Buffer
      ,  u32LengthBuffer
   );
}

void Type_SwcServiceEthTp::vWrite(
      const sint8* pcs8Buffer
   ,        uint32 u32LengthBuffer
){
   send(
         this->stChannel.s32Socket
      ,  pcs8Buffer
      ,  u32LengthBuffer
      ,  this->stCfg.u32FlagsSend
   );
}

void Type_SwcServiceEthTp::vMainFunction(void){
}

/******************************************************************************/
/* EOF                                                                        */
/******************************************************************************/
