// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2012 The Bitcoin developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "assert.h"

#include "chainparams.h"
#include "core.h"
#include "protocol.h"
#include "util.h"

//
// Main network
//

unsigned int pnSeed[] =
{
    0x12345678
};

class CMainParams : public CChainParams {
public:
    CMainParams() {
        // The message start string is designed to be unlikely to occur in normal data.
        pchMessageStart[0] = 0x0A;
        pchMessageStart[1] = 0x05;
        pchMessageStart[2] = 0x03;
        pchMessageStart[3] = 0x0C;
        nDefaultPort = 10240;
        nRPCPort = 10241;
        bnProofOfWorkLimit[ALGO_SHA256D] = CBigNum(~uint256(0) >> 20);
        bnProofOfWorkLimit[ALGO_SCRYPT]  = CBigNum(~uint256(0) >> 20);
        bnProofOfWorkLimit[ALGO_GROESTL] = CBigNum(~uint256(0) >> 20);
        bnProofOfWorkLimit[ALGO_SKEIN]   = CBigNum(~uint256(0) >> 20);
        bnProofOfWorkLimit[ALGO_QUBIT]   = CBigNum(~uint256(0) >> 20);
        nSubsidyHalvingInterval = 102400;

        // Build the genesis block. Note that the output of the genesis coinbase cannot
        // be spent as it did not originally exist in the database.
  
        const char* pszTimestamp = "An empire is born";
        CTransaction txNew;
        txNew.vin.resize(1);
        txNew.vout.resize(1);
        txNew.vin[0].scriptSig = CScript() << 486604799 << CBigNum(4) << vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
        txNew.vout[0].nValue = 1 * COIN;
        genesis.vtx.push_back(txNew);
        genesis.hashPrevBlock = 0;
        genesis.hashMerkleRoot = genesis.BuildMerkleTree();
        genesis.nVersion = BLOCK_VERSION_DEFAULT;
        genesis.nTime    = 1393638000;
        genesis.nBits    = 0x1e0fffff;
        genesis.nNonce   = 1384805;
        
        //// debug print
        hashGenesisBlock = genesis.GetHash();
        while (hashGenesisBlock > bnProofOfWorkLimit[ALGO_SHA256D].getuint256()){
            if (++genesis.nNonce==0) break;
            hashGenesisBlock = genesis.GetHash();
        }

        printf("MAIN: %s\n", hashGenesisBlock.ToString().c_str());
        printf("%s\n", genesis.hashMerkleRoot.ToString().c_str());
        printf("%x\n", bnProofOfWorkLimit[ALGO_SHA256D].GetCompact());
        genesis.print();
        
        assert(hashGenesisBlock == uint256("0x000000e6bd36be354023671ebdd223503ce0c496eb985967377e8245af6d5581"));
        assert(genesis.hashMerkleRoot == uint256("0x761a1f52243fb883bdadce442a50175bd56f3c40786ca8f562ee886be084113b"));
		
        base58Prefixes[PUBKEY_ADDRESS] = 1;
        base58Prefixes[SCRIPT_ADDRESS] = 9;
        base58Prefixes[SECRET_KEY] = 178;

        // Convert the pnSeeds array into usable address objects.
        for (unsigned int i = 0; i < ARRAYLEN(pnSeed); i++)
        {
            // It'll only connect to one or two seed nodes because once it connects,
            // it'll get a pile of addresses with newer timestamps.
            // Seed nodes are given a random 'last seen time' 
            const int64 nTwoDays = 2 * 24 * 60 * 60;
            struct in_addr ip;
            memcpy(&ip, &pnSeed[i], sizeof(ip));
            CAddress addr(CService(ip, GetDefaultPort()));
            addr.nTime = GetTime() - GetRand(nTwoDays) - nTwoDays;
            vFixedSeeds.push_back(addr);
        }
    }

    virtual const CBlock& GenesisBlock() const { return genesis; }
    virtual Network NetworkID() const { return CChainParams::MAIN; }

    virtual const vector<CAddress>& FixedSeeds() const {
        return vFixedSeeds;
    }
protected:
    CBlock genesis;
    vector<CAddress> vFixedSeeds;
};
static CMainParams mainParams;


//
// Testnet (v3)
//
class CTestNetParams : public CMainParams {
public:
    CTestNetParams() {
        // The message start string is designed to be unlikely to occur in normal data.
        pchMessageStart[0] = 0x01;
        pchMessageStart[1] = 0xf5;
        pchMessageStart[2] = 0x55;
        pchMessageStart[3] = 0xa4;
        vAlertPubKey = ParseHex("044adf046e6bc86fb83ef92f261fa3feff9176bd029c5ad4afb5c52ac21f9851f2b2eb861cdbf2c09e0cb97dbf75c6ca5ff6c5df88cfb244c72dba1d44b5a47655");
        nDefaultPort = 20888;
        nRPCPort = 20889;
        strDataDir = "testnet";

        // Modify the testnet genesis block so the timestamp is valid for a later start.
        genesis.nTime = 1392876393;
        genesis.nNonce = 416875379;
        
        //// debug print
        hashGenesisBlock = genesis.GetHash();
        //while (hashGenesisBlock > bnProofOfWorkLimit[ALGO_SHA256D].getuint256()){
        //    if (++genesis.nNonce==0) break;
        //   hashGenesisBlock = genesis.GetHash();
        //}

        //printf("TEST: %s\n", hashGenesisBlock.ToString().c_str());
        //printf("%s\n", genesis.hashMerkleRoot.ToString().c_str());
        //genesis.print();
        
        assert(hashGenesisBlock == uint256("0x0000017ce2a79c8bddafbbe47c004aa92b20678c354b34085f62b762084b9788"));

        vFixedSeeds.clear();
        vSeeds.clear();
        vSeeds.push_back(CDNSSeedData("68.232.180.111", "68.232.180.111"));

        base58Prefixes[PUBKEY_ADDRESS] = 88;
        base58Prefixes[SCRIPT_ADDRESS] = 188;
        base58Prefixes[SECRET_KEY] = 239;

    }
    virtual Network NetworkID() const { return CChainParams::TESTNET; }
};
static CTestNetParams testNetParams;


//
// Regression test
//
class CRegTestParams : public CTestNetParams {
public:
    CRegTestParams() {
        pchMessageStart[0] = 0xfa;
        pchMessageStart[1] = 0x0f;
        pchMessageStart[2] = 0xa5;
        pchMessageStart[3] = 0x5a;
        nSubsidyHalvingInterval = 150;
        bnProofOfWorkLimit[ALGO_SHA256D] = CBigNum(~uint256(0) >> 1);
        bnProofOfWorkLimit[ALGO_SCRYPT]  = CBigNum(~uint256(0) >> 1);
        bnProofOfWorkLimit[ALGO_GROESTL] = CBigNum(~uint256(0) >> 1);
        bnProofOfWorkLimit[ALGO_SKEIN]   = CBigNum(~uint256(0) >> 1);
        bnProofOfWorkLimit[ALGO_QUBIT]   = CBigNum(~uint256(0) >> 1);
        genesis.nTime = 1296688602;
        genesis.nBits = 0x207fffff;
        genesis.nNonce = 4;
        hashGenesisBlock = genesis.GetHash();
        nDefaultPort = 18444;
        strDataDir = "regtest";
        
        //// debug print
        hashGenesisBlock = genesis.GetHash();
        //while (hashGenesisBlock > bnProofOfWorkLimit[ALGO_SHA256D].getuint256()){
        //    if (++genesis.nNonce==0) break;
        //    hashGenesisBlock = genesis.GetHash();
        //}

        //printf("REG: %s\n", hashGenesisBlock.ToString().c_str());
        //printf("%s\n", genesis.hashMerkleRoot.ToString().c_str());
        //genesis.print();

        assert(hashGenesisBlock == uint256("0x13d8d31dde96874006da503dd2b63fa68c698dc823334359e417aa3a92f80433"));

        vSeeds.clear();  // Regtest mode doesn't have any DNS seeds.

        base58Prefixes[PUBKEY_ADDRESS] = 0;
        base58Prefixes[SCRIPT_ADDRESS] = 5;
        base58Prefixes[SECRET_KEY] = 128;
    }

    virtual bool RequireRPCPassword() const { return false; }
    virtual Network NetworkID() const { return CChainParams::REGTEST; }
};
static CRegTestParams regTestParams;

static CChainParams *pCurrentParams = &mainParams;

const CChainParams &Params() {
    return *pCurrentParams;
}

void SelectParams(CChainParams::Network network) {
    switch (network) {
        case CChainParams::MAIN:
            pCurrentParams = &mainParams;
            break;
        case CChainParams::TESTNET:
            pCurrentParams = &testNetParams;
            break;
        case CChainParams::REGTEST:
            pCurrentParams = &regTestParams;
            break;
        default:
            assert(false && "Unimplemented network");
            return;
    }
}

bool SelectParamsFromCommandLine() {
    bool fRegTest = GetBoolArg("-regtest", false);
    bool fTestNet = GetBoolArg("-testnet", false);

    if (fTestNet && fRegTest) {
        return false;
    }

    if (fRegTest) {
        SelectParams(CChainParams::REGTEST);
    } else if (fTestNet) {
        SelectParams(CChainParams::TESTNET);
    } else {
        SelectParams(CChainParams::MAIN);
    }
    return true;
}
