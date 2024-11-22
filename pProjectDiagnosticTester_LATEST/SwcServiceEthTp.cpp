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
#include <arpa/inet.h>
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
   uint32 u32Socket;
}Type_McalEth_stChannel;

class Type_SwcServiceEthTp:
      public infClientSwcServiceEthTp
{
   private:
                     Type_McalEth_stChannel stChannel;
      volatile const Type_CfgMcalEth*       pvcstCfg;

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
   this->pvcstCfg = &CfgMcalEth;
   if(
         0
      >  (
            this->stChannel.u32Socket = socket(
                  this->pvcstCfg->stSocket.u32Domain
               ,  this->pvcstCfg->stSocket.u32SocketType
               ,  this->pvcstCfg->stSocket.u32Protocol
            )
         )
   ){
      std::cerr << "socket" << std::endl;
      exit(EXIT_FAILURE);
   }

   if(
         0
      >  connect(
                                   this->stChannel.u32Socket
            ,  (struct sockaddr*) &this->pvcstCfg->stAddress
            ,  (socklen_t)         this->pvcstCfg->tSizeAddress
         )
   ){
      std::cerr << "connect" << std::endl;
      exit(EXIT_FAILURE);
   }
}

void Type_SwcServiceEthTp::vDeInitFunction(void){
   close(this->stChannel.u32Socket);
}

void Type_SwcServiceEthTp::vRead(
      sint8* ps8Buffer
   ,  uint32 u32LengthBuffer
){
   read(
         this->stChannel.u32Socket
      ,  ps8Buffer
      ,  u32LengthBuffer
   );
}

void Type_SwcServiceEthTp::vWrite(
      const sint8* pcs8Buffer
   ,        uint32 u32LengthBuffer
){
   send(
         this->stChannel.u32Socket
      ,  pcs8Buffer
      ,  u32LengthBuffer
      ,  this->pvcstCfg->u32FlagsSend
   );
}

void Type_SwcServiceEthTp::vMainFunction(void){
}

/******************************************************************************/
/* EOF                                                                        */
/******************************************************************************/
