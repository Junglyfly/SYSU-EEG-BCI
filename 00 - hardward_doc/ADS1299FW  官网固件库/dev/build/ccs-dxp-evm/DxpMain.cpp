/*!
 * ============================================================================
 *
 * Copyright 2004-2011, Fuel7, Inc. All Rights Reserved
 * This software is the confidential and proprietary information of Fuel7, Inc.
 * ("Confidential Information").  You shall not disclose such Confidential
 * Information and shall use it only in accordance with the terms of the
 * license agreement you entered into with Fuel7.
 *
 * FUEL7 MAKES NO REPRESENTATIONS OR WARRANTIES ABOUT THE SUITABILITY OF THE
 * SOFTWARE, EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, OR
 * NON-INFRINGEMENT. FUEL7 SHALL NOT BE LIABLE FOR ANY DAMAGES SUFFERED BY
 * LICENSEE AS A RESULT OF USING, MODIFYING OR DISTRIBUTING THIS SOFTWARE OR
 * ITS DERIVATIVES.
 *
 * CopyrightVersion 1.0.15
 *
 * @brief The startup procedure for the MMB0 Server
 *
 *  This procedure initiates the DXP Next Generation program. 
 *  DxpMain.cpp performs the creation of the Mmb0Server which then creates 
 *  the devices and associated drivers. This program runs on top of the 
 *  DSP/BIOS and uses both the 55xx CSL and MMB0 libraries. 
 *
 * @date 2011/12/21
 * 
 * @author Eric Dixon
 *
 * ============================================================================
 */


#include <server/mmb0dxp/Mmb0Server.h>

using namespace com_fuel7_mp_cpp;
using namespace com_fuel7_mp_cpp::dxp::server::mmb0;

static CMmb0Server* g_pServer;



int main()
{
    g_pServer = new CMmb0Server();

    g_pServer->Create();

    g_pServer->Startup();
}

