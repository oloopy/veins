//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
//

#ifndef __MIXNETBRIDGE_H__
#define __MIXNETBRIDGE_H__

#include <omnetpp.h>
#include <MACAddress.h>
#include <BaseLayer.h>
#include <MIxNETWorldUtility.h>

/**
 * @brief INET <-> MiXiM compatibility class. Converts packet between
 * INET network layer and MiXiM NIC.
 *
 * For support of MiXiM NICs in INET hosts put this connect the MiXiM
 * NIC with the lower gates of the bridge and the INET network layer
 * with the upper gates of this bridge.
 *
 * Does the following things:
 * - registers the the NIC connected to its lowerGateOut in INETs Interface
 * table
 * - dumps control messages send by the NIC (INET does support them)
 * - forwards packets from NIC to upper layer
 * - converts the Ieee802Ctrl control info (INET) of packets send from upper layer to NIC
 *   to a NetwToMacControlInfo (INET)
 * - converts INET MAC addresses to MiXiM MAC addresses
 *
 * This class expects that the MiXiM NIC uses the NIC modules id as MAC address to work.
 * Therefore no addressing module implementing MiXiMs "AddressingInterface" should be
 * present in the host!
 *
 * Uses MIxNETWorldUtility as INET<->MiXiM MAC address database.
 *
 * @ingroup mixnet
 *
 * @author KarlWessel
 */
class MixnetBridge : public cSimpleModule
{
protected:
	/** @brief INET's MAC address for this bridges NIC.*/
	MACAddress myINETMacAddr;
	/** @brief MiXiM's MAC address for this bridges NIC.*/
	int myMiximMacAddr;

	/** @brief Pointer to MIxNET's world utility module.*/
	MIxNETWorldUtility* world;

	/** @brief Pointer to this bridge's NIC module.*/
	cModule* nic;

	/** @name gate ids*/
    /*@{*/
    int upperGateIn;
    int upperGateOut;
    int lowerGateIn;
    int lowerGateOut;
    int lowerControlIn;
    int lowerControlOut;

    /*@}*/

public:
    virtual void initialize(int stage);
    virtual void handleMessage(cMessage *msg);

    virtual int numInitStages() const {return 2;}

protected:
    /**
     * @brief Registers this bridges NIC with INET's InterfaceTable.
     */
    void registerInterface();

    /** @brief Handle messages from upper layer
     */
    virtual void handleUpperMsg(cMessage *msg);

    /** @brief Handle messages from lower layer */
    virtual void handleLowerMsg(cMessage *msg);

    /**
     * @brief Looks for this bridges module by using the
     * lowerGateOut connection.
     *
     * @return Pointer to this bridges NIC module or NULL
     */
    cModule* findMyNic();
};

#endif
