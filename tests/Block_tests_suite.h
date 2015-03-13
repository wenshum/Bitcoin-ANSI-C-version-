/******----------------------- File Name: Block_tests_suite.h ---------------------------******

Created by: Fadwa & Senka on 19/11/2012.
Tested by: Full Name on DD/MM/YYYY.
Modified by: Fadwa & Senka on 21/11/2012.
Version: 1.0
Copyright (c) 2012, MIBitcoinc


Brief Description:
------------------
This file includes all the test cases related to Block. It inlcudes functions:
test_case_genesisBlock(), test_case_genesisBlockSerialisation(), test_case_calculateBlockHash(),
test_case_createNewBlock(), test_case_destroyBlock()

******--------------------------------------------------------------------******/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "CUnit/CUnit.h"
#include "CUnit/Basic.h"
/*#include "CUnit/TBTest.h"*/
/*#include "CUnit/Automated.h"
 #include "CUnit/Console.h"*/
#include "../src/Object/Message/Block.h"
#include "../src/Object/ByteArray.h"
#include "../src/Constants.h"
#include "openssl/ssl.h"
#include "openssl/ripemd.h"
#include "openssl/rand.h"
#include "../src/Utils/VariableLengthInteger/VarLenInt.h"

void err1(Error a, char * format, ...);
void err1(Error a, char * format, ...) {
	va_list argptr;
	va_start(argptr, format);
	vfprintf(stderr, format, argptr);
	va_end(argptr);
	printf("\n");
}

/* Test Suite setup and cleanup functions: */

int init_Block_suite(void) {
	return 0;
}

int clean_Block_suite(void) {
	return 0;
}

/************* Test case functions ****************/

/* Test genesis block */

void test_case_genesisBlock(void){

		unsigned int s = (unsigned int)time(NULL);
		s = 1337544566;
		srand(s);

	ByteArray * genesisMerkleRoot = createNewByteArrayUsingDataCopy((uint8_t []){0x3B,0xA3,0xED,0xFD,0x7A,0x7B,0x12,0xB2,0x7A,0xC7,0x2C,0x3E,0x67,0x76,0x8F,0x61,0x7F,0xC8,0x1B,0xC3,0x88,0x8A,0x51,0x32,0x3A,0x9F,0xB8,0xAA,0x4B,0x1E,0x5E,0x4A}, 32, err1);

	ByteArray * genesisInScript = createNewByteArrayUsingDataCopy((uint8_t [77]){0x04,0xFF,0xFF,0x00,0x1D,0x01,0x04,0x45,0x54,0x68,0x65,0x20,0x54,0x69,0x6D,0x65,0x73,0x20,0x30,0x33,0x2F,0x4A,0x61,0x6E,0x2F,0x32,0x30,0x30,0x39,0x20,0x43,0x68,0x61,0x6E,0x63,0x65,0x6C,0x6C,0x6F,0x72,0x20,0x6F,0x6E,0x20,0x62,0x72,0x69,0x6E,0x6B,0x20,0x6F,0x66,0x20,0x73,0x65,0x63,0x6F,0x6E,0x64,0x20,0x62,0x61,0x69,0x6C,0x6F,0x75,0x74,0x20,0x66,0x6F,0x72,0x20,0x62,0x61,0x6E,0x6B,0x73}, 77, err1);

	ByteArray * genesisOutScript = createNewByteArrayUsingDataCopy((uint8_t [67]){0x41,0x04,0x67,0x8A,0xFD,0xB0,0xFE,0x55,0x48,0x27,0x19,0x67,0xF1,0xA6,0x71,0x30,0xB7,0x10,0x5C,0xD6,0xA8,0x28,0xE0,0x39,0x09,0xA6,0x79,0x62,0xE0,0xEA,0x1F,0x61,0xDE,0xB6,0x49,0xF6,0xBC,0x3F,0x4C,0xEF,0x38,0xC4,0xF3,0x55,0x04,0xE5,0x1E,0xC1,0x12,0xDE,0x5C,0x38,0x4D,0xF7,0xBA,0x0B,0x8D,0x57,0x8A,0x4C,0x70,0x2B,0x6B,0xF1,0x1D,0x5F,0xAC}, 67, err1);

	Block *genesisBlock = createNewBlockFromOriginalBlock(err1);

	/*Test the creation of genesis block*/
	CU_ASSERT_PTR_NOT_NULL(genesisMerkleRoot);
	CU_ASSERT_PTR_NOT_NULL(genesisInScript);
	CU_ASSERT_PTR_NOT_NULL(genesisOutScript);
	CU_ASSERT_PTR_NOT_NULL(genesisBlock);
	CU_ASSERT_EQUAL(genesisBlock->version, 1);

	int x;
		for ( x = 0; x < 32; x++)
		{
			CU_ASSERT_EQUAL(getByteFromByteArray(genesisBlock->previousBlockHash, x), 0);
		}

	CU_ASSERT_EQUAL(compareByteArrays(genesisBlock->merkleTreeRootHash, genesisMerkleRoot), 0);
	CU_ASSERT_EQUAL(genesisBlock->time, 1231006505);
	CU_ASSERT_EQUAL(genesisBlock->target, 0x1D00FFFF);
	CU_ASSERT_EQUAL(genesisBlock->nonce, 2083236893);
	CU_ASSERT_EQUAL(genesisBlock->numOfTransactions, 1);


	/*test the transaction in genesis Block*/
	Transaction * genesisCoinBase = genesisBlock->transactions[0];
	CU_ASSERT_EQUAL(genesisCoinBase->numOfTransactionInput, 1);
	CU_ASSERT_EQUAL(genesisCoinBase->numOfTransactionOutput, 1);
	CU_ASSERT_EQUAL(genesisCoinBase->version, 1);
	CU_ASSERT_EQUAL(genesisCoinBase->lockTime, 0);
	CU_ASSERT_EQUAL(genesisCoinBase->inputs[0]->scriptObject->length, 0x4D);
	CU_ASSERT_EQUAL(genesisCoinBase->outputs[0]->scriptObject->length, 0x43);

	for (x = 0; x < 32; x++)
	{
		CU_ASSERT_EQUAL(getByteFromByteArray(genesisCoinBase->inputs[0]->prevOutput.hash, x), 0);
	}


	CU_ASSERT_EQUAL(genesisCoinBase->inputs[0]->prevOutput.index, TRANSACTION_INPUT_FINAL);
	CU_ASSERT_EQUAL(genesisCoinBase->inputs[0]->sequence, TRANSACTION_INPUT_FINAL);
	CU_ASSERT_EQUAL(compareByteArrays(genesisCoinBase->inputs[0]->scriptObject, genesisInScript), 0);
	CU_ASSERT_EQUAL(genesisCoinBase->outputs[0]->value, 5000000000);
	CU_ASSERT_EQUAL(compareByteArrays(genesisCoinBase->outputs[0]->scriptObject, genesisOutScript), 0);
}



	/* Test serialisation into genesis block*/
void test_case_genesisBlockSerialisation(void)
{
	ByteArray * genesisMerkleRoot = createNewByteArrayUsingDataCopy((uint8_t []){0x3B,0xA3,0xED,0xFD,0x7A,0x7B,0x12,0xB2,0x7A,0xC7,0x2C,0x3E,0x67,0x76,0x8F,0x61,0x7F,0xC8,0x1B,0xC3,0x88,0x8A,0x51,0x32,0x3A,0x9F,0xB8,0xAA,0x4B,0x1E,0x5E,0x4A}, 32, err1);

	ByteArray * genesisInScript = createNewByteArrayUsingDataCopy((uint8_t [77]){0x04,0xFF,0xFF,0x00,0x1D,0x01,0x04,0x45,0x54,0x68,0x65,0x20,0x54,0x69,0x6D,0x65,0x73,0x20,0x30,0x33,0x2F,0x4A,0x61,0x6E,0x2F,0x32,0x30,0x30,0x39,0x20,0x43,0x68,0x61,0x6E,0x63,0x65,0x6C,0x6C,0x6F,0x72,0x20,0x6F,0x6E,0x20,0x62,0x72,0x69,0x6E,0x6B,0x20,0x6F,0x66,0x20,0x73,0x65,0x63,0x6F,0x6E,0x64,0x20,0x62,0x61,0x69,0x6C,0x6F,0x75,0x74,0x20,0x66,0x6F,0x72,0x20,0x62,0x61,0x6E,0x6B,0x73}, 77, err1);

	ByteArray * genesisOutScript = createNewByteArrayUsingDataCopy((uint8_t [67]){0x41,0x04,0x67,0x8A,0xFD,0xB0,0xFE,0x55,0x48,0x27,0x19,0x67,0xF1,0xA6,0x71,0x30,0xB7,0x10,0x5C,0xD6,0xA8,0x28,0xE0,0x39,0x09,0xA6,0x79,0x62,0xE0,0xEA,0x1F,0x61,0xDE,0xB6,0x49,0xF6,0xBC,0x3F,0x4C,0xEF,0x38,0xC4,0xF3,0x55,0x04,0xE5,0x1E,0xC1,0x12,0xDE,0x5C,0x38,0x4D,0xF7,0xBA,0x0B,0x8D,0x57,0x8A,0x4C,0x70,0x2B,0x6B,0xF1,0x1D,0x5F,0xAC}, 67, err1);

	Block *genesisBlock = createNewBlockFromOriginalBlock(err1);

	Block * block = createNewBlock(err1);
	block->version = 1;
	uint8_t * zeroHash = malloc(32);
	memset(zeroHash, 0, 32);
	block->previousBlockHash = createNewByteArrayFromData(zeroHash, 32, err1);
		block->merkleTreeRootHash = genesisMerkleRoot;
		block->target = 0x1D00FFFF;
		block->time = 1231006505;
		block->nonce = 2083236893;
		block->numOfTransactions = 1;
		block->transactions = malloc(sizeof(*block->transactions));
		block->transactions[0] = createNewTransaction(0, 1, err1);
		incrementReferenceCount(block->previousBlockHash); 		/* Retain for the zero hash in the input*/

		takeTransactionInput(block->transactions[0], createNewTransactionInput(genesisInScript, 0xFFFFFFFF, block->previousBlockHash, 0xFFFFFFFF, err1));
		takeTransactionOutput(block->transactions[0], createNewTransactionOutput(5000000000, genesisOutScript, err1));
		getMessage(block)->bytes = createNewByteArrayOfSize(getMessage(genesisBlock)->bytes->length, err1);

		/* to be checked with Popoola */
		CU_ASSERT_NOT_EQUAL(serializeBlock(block, TRUE), FALSE);
		CU_ASSERT_EQUAL(compareByteArrays(getMessage(block)->bytes, getMessage(genesisBlock)->bytes), 0);

}

/* Test Hash */
void test_case_calculateBlockHash(void)
{
	Block *genesisBlock = createNewBlockFromOriginalBlock(err1);
	uint8_t calcHash[32];
	calculateBlockHash(genesisBlock,calcHash);
	CU_ASSERT_PTR_NOT_NULL(genesisBlock->hash);
	CU_ASSERT_EQUAL(memcmp(genesisBlock->hash, calcHash,32), 0);
}


void test_case_createNewBlock(void)
{
	Block *newBlock = createNewBlock(err1);

	CU_ASSERT_EQUAL(newBlock->previousBlockHash, NULL);
	CU_ASSERT_EQUAL(newBlock->merkleTreeRootHash, NULL);
	CU_ASSERT_EQUAL(newBlock->transactions, NULL);
	CU_ASSERT_EQUAL(newBlock->numOfTransactions, 0);
	CU_ASSERT_EQUAL(getMessage(newBlock)->bytes, NULL);
	CU_ASSERT_EQUAL(getMessage(newBlock)->onErrorReceived, err1);
	CU_ASSERT_EQUAL(getMessage(newBlock)->expectResponse, FALSE);
	CU_ASSERT_PTR_NOT_NULL(newBlock);

}


void test_case_destroyBlock(void)
{
	ByteArray * genesisMerkleRoot = createNewByteArrayUsingDataCopy((uint8_t []){0x3B,0xA3,0xED,0xFD,0x7A,0x7B,0x12,0xB2,0x7A,0xC7,0x2C,0x3E,0x67,0x76,0x8F,0x61,0x7F,0xC8,0x1B,0xC3,0x88,0x8A,0x51,0x32,0x3A,0x9F,0xB8,0xAA,0x4B,0x1E,0x5E,0x4A}, 32, err1);

	ByteArray * genesisInScript = createNewByteArrayUsingDataCopy((uint8_t [77]){0x04,0xFF,0xFF,0x00,0x1D,0x01,0x04,0x45,0x54,0x68,0x65,0x20,0x54,0x69,0x6D,0x65,0x73,0x20,0x30,0x33,0x2F,0x4A,0x61,0x6E,0x2F,0x32,0x30,0x30,0x39,0x20,0x43,0x68,0x61,0x6E,0x63,0x65,0x6C,0x6C,0x6F,0x72,0x20,0x6F,0x6E,0x20,0x62,0x72,0x69,0x6E,0x6B,0x20,0x6F,0x66,0x20,0x73,0x65,0x63,0x6F,0x6E,0x64,0x20,0x62,0x61,0x69,0x6C,0x6F,0x75,0x74,0x20,0x66,0x6F,0x72,0x20,0x62,0x61,0x6E,0x6B,0x73}, 77, err1);

	ByteArray * genesisOutScript = createNewByteArrayUsingDataCopy((uint8_t [67]){0x41,0x04,0x67,0x8A,0xFD,0xB0,0xFE,0x55,0x48,0x27,0x19,0x67,0xF1,0xA6,0x71,0x30,0xB7,0x10,0x5C,0xD6,0xA8,0x28,0xE0,0x39,0x09,0xA6,0x79,0x62,0xE0,0xEA,0x1F,0x61,0xDE,0xB6,0x49,0xF6,0xBC,0x3F,0x4C,0xEF,0x38,0xC4,0xF3,0x55,0x04,0xE5,0x1E,0xC1,0x12,0xDE,0x5C,0x38,0x4D,0xF7,0xBA,0x0B,0x8D,0x57,0x8A,0x4C,0x70,0x2B,0x6B,0xF1,0x1D,0x5F,0xAC}, 67, err1);

	Block *genesisBlock = createNewBlockFromOriginalBlock(err1);

	Block * block = createNewBlock(err1);
	block->version = 1;
	uint8_t * zeroHash = malloc(32);
	memset(zeroHash, 0, 32);
	block->previousBlockHash = createNewByteArrayFromData(zeroHash, 32, err1);
	block->merkleTreeRootHash = genesisMerkleRoot;
	block->target = 0x1D00FFFF;
	block->time = 1231006505;
	block->nonce = 2083236893;
	block->numOfTransactions = 1;
	block->transactions = malloc(sizeof(*block->transactions));
	block->transactions[0] = createNewTransaction(0, 1, err1);
	incrementReferenceCount(block->previousBlockHash); 		/* Retain for the zero hash in the input*/

	takeTransactionInput(block->transactions[0], createNewTransactionInput(genesisInScript, 0xFFFFFFFF, block->previousBlockHash, 0xFFFFFFFF, err1));
	takeTransactionOutput(block->transactions[0], createNewTransactionOutput(5000000000, genesisOutScript, err1));
	getMessage(block)->bytes = createNewByteArrayOfSize(getMessage(genesisBlock)->bytes->length, err1);

	destroyBlock(block);

	/*CU_ASSERT_EQUAL(block, NULL);*/
}

/* Grouping all test cases into an array that will be registered later with a single test suite. */
CU_TestInfo block_test_array[] = {
  { "test_case_genesisBlock", test_case_genesisBlock },
  { "test_case_genesisBlockSerialisation", test_case_genesisBlockSerialisation },
  { "test_case_calculateBlockHash", test_case_calculateBlockHash },
  { "test_case_createNewBlock", test_case_createNewBlock },
  { "test_case_destroyBlock", test_case_destroyBlock},
  CU_TEST_INFO_NULL,
};

