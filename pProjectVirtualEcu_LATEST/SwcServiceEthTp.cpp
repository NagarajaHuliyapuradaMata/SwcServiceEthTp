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

#include <iostream> // TBD: optimize
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>

#include "infClientCfgMcalEth.hpp"

/******************************************************************************/
/* #DEFINES                                                                   */
/******************************************************************************/

/******************************************************************************/
/* MACROS                                                                     */
/******************************************************************************/

/******************************************************************************/
/* TYPEDEFS                                                                   */
/******************************************************************************/

/******************************************************************************/
/* CONSTS                                                                     */
/******************************************************************************/

/******************************************************************************/
/* PARAMS                                                                     */
/******************************************************************************/
typedef struct{
   sint32 s32FileDescriptorSocket;
   sint32 s32Socket;
}Type_McalEth_stChannel;

class Type_SwcServiceEthTp:
      public infClientSwcServiceEthTp
{
   private:
      Type_McalEth_stChannel stChannel;
      Type_CfgMcalEth*       pstCfg;

   public:
      void vInitFunction   (void);
      void vRead           (      sint8* ps8Buffer,  uint32 u32LengthBuffer);
      void vWrite          (const sint8* pcs8Buffer, uint32 u32LengthBuffer);
      void vMainFunction   (void);
      void vDeInitFunction (void);
};

/******************************************************************************/
/* OBJECTS                                                                    */
/******************************************************************************/
Type_SwcServiceEthTp            SwcServiceEthTp;
infClientSwcServiceEthTp* const cpstinfClientSwcServiceEthTp = &SwcServiceEthTp;

/******************************************************************************/
/* FUNCTIONS                                                                  */
/******************************************************************************/
void Type_SwcServiceEthTp::vInitFunction(void){
   this->pstCfg = CfgMcalEth_pstGet();
   if(
         0
      == (
            this->stChannel.s32FileDescriptorSocket = socket(
                  this->pstCfg->stSocket.u32Domain
               ,  this->pstCfg->stSocket.u32SocketType
               ,  this->pstCfg->stSocket.u32Protocol
            )
         )
   ){
      std::cerr << std::endl << "ERROR: socket creation failed" << std::endl;
      exit(EXIT_FAILURE);
   }

   if(
      setsockopt(
                           this->stChannel.s32FileDescriptorSocket
         ,                 this->pstCfg->stOption.u32Level
         ,                 this->pstCfg->stOption.u32OptionName
         ,  (const void*) &this->pstCfg->stOption.u32Option
         ,                 this->pstCfg->stOption.tSizeOption
      )
   ){
      std::cerr << std::endl << "ERROR: setsockopt" << std::endl;
      exit(EXIT_FAILURE);
   }

   if(
         0
      >  bind(
                                   this->stChannel.s32FileDescriptorSocket
            ,  (struct sockaddr*) &this->pstCfg->stAddress
            ,                      this->pstCfg->tSizeAddress
         )
   ){
      std::cerr << std::endl << "ERROR: bind failed" << std::endl;
      exit(EXIT_FAILURE);
   }

   if(
         0
      >  listen(
               this->stChannel.s32FileDescriptorSocket
            ,  this->pstCfg->u32BacklogListen
         )
   ){
      std::cerr << std::endl << "ERROR: listen" << std::endl;
      exit(EXIT_FAILURE);
   }

   if(
         0
      >  (
            this->stChannel.s32Socket = accept(
                                      this->stChannel.s32FileDescriptorSocket
               ,  (struct sockaddr*) &this->pstCfg->stAddress
               ,  (socklen_t*)       &this->pstCfg->tSizeAddress
            )
         )
   ){
      std::cerr << std::endl << "ERROR: accept" << std::endl;
      exit(EXIT_FAILURE);
   }
}

void Type_SwcServiceEthTp::vDeInitFunction(void){
   close(this->stChannel.s32Socket);
   close(this->stChannel.s32FileDescriptorSocket);
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
      ,  this->pstCfg->u32FlagsSend
   );
}

void Type_SwcServiceEthTp::vMainFunction(void){
}

/******************************************************************************/
/* EOF                                                                        */
/******************************************************************************/
