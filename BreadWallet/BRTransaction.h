//
//  BRTransaction.h
//  TosWallet
//
//  Created by Aaron Voisine on 5/16/13.
//  Copyright (c) 2013 Aaron Voisine <voisine@gmail.com>
//  Copyright © 2016 Litecoin Association <loshan1212@gmail.com>
//  Copyright (c) 2018 Blockware Corp. <admin@blockware.co.kr>
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to deal
//  in the Software without restriction, including without limitation the rights
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//  copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in
//  all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//  THE SOFTWARE.

#import <Foundation/Foundation.h>

#define TX_FEE_PER_KB        1000ULL     // standard tx fee per kb of tx size, rounded up to nearest kb
#define TX_OUTPUT_SIZE       34          // estimated size for a typical transaction output
#define TX_INPUT_SIZE        148         // estimated size for a typical compact pubkey transaction input
#define TX_MIN_OUTPUT_AMOUNT 1          //no txout can be below this amount
/* TX_MIN_OUTPUT_AMOUNT
 1 = 0.000001 Tos
 10 = 0.00001 Tos
 100 = 0.0001 Tos
 1000 = 0.001 Tos
 10000 = 0.01 Tos
 100000 = 0.1 Tos
 1000000 = 1  Tos
*/
#define TX_MAX_SIZE          100000      // no tx can be larger than this size in bytes
#define TX_FREE_MAX_SIZE     0        // tx must not be larger than this size in bytes without a fee
#define TX_FREE_MIN_PRIORITY 57600000ULL // tx must not have a priority below this value without a fee
#define TX_UNCONFIRMED       INT32_MAX   // block height indicating transaction is unconfirmed
#define TX_MAX_LOCK_HEIGHT   500000000   // a lockTime below this value is a block height, otherwise a timestamp

typedef union _UInt256 UInt256;

@interface BRTransaction : NSObject

@property (nonatomic, readonly) NSArray *inputAddresses;
@property (nonatomic, readonly) NSArray *inputHashes;
@property (nonatomic, readonly) NSArray *inputIndexes;
@property (nonatomic, readonly) NSArray *inputScripts;
@property (nonatomic, readonly) NSArray *inputSignatures;
@property (nonatomic, readonly) NSArray *inputSequences;
@property (nonatomic, readonly) NSArray *outputAmounts;
@property (nonatomic, readonly) NSArray *outputAddresses;
@property (nonatomic, readonly) NSArray *outputScripts;

@property (nonatomic, assign) UInt256 txHash;
@property (nonatomic, assign) uint32_t version;
@property (nonatomic, assign) uint32_t lockTime;
@property (nonatomic, assign) uint32_t blockHeight;
@property (nonatomic, assign) NSTimeInterval timestamp; // time interval since refrence date, 00:00:00 01/01/01 GMT
@property (nonatomic, readonly) size_t size; // size in bytes if signed, or estimated size assuming compact pubkey sigs
@property (nonatomic, readonly) uint64_t standardFee;
@property (nonatomic, readonly) BOOL isSigned; // checks if all signatures exist, but does not verify them
@property (nonatomic, readonly, getter = toData) NSData *data;

@property (nonatomic, readonly) NSString *longDescription;

+ (instancetype)transactionWithMessage:(NSData *)message;

- (instancetype)initWithMessage:(NSData *)message;
- (instancetype)initWithInputHashes:(NSArray *)hashes inputIndexes:(NSArray *)indexes inputScripts:(NSArray *)scripts
outputAddresses:(NSArray *)addresses outputAmounts:(NSArray *)amounts;

- (void)addInputHash:(UInt256)hash index:(NSUInteger)index script:(NSData *)script;
- (void)addInputHash:(UInt256)hash index:(NSUInteger)index script:(NSData *)script signature:(NSData *)signature
sequence:(uint32_t)sequence;
- (void)addOutputAddress:(NSString *)address amount:(uint64_t)amount;
- (void)addOutputScript:(NSData *)script amount:(uint64_t)amount;
- (void)setInputAddress:(NSString *)address atIndex:(NSUInteger)index;
- (void)shuffleOutputOrder;
- (BOOL)signWithPrivateKeys:(NSArray *)privateKeys;

// priority = sum(input_amount_in_satoshis*input_age_in_blocks)/tx_size_in_bytes
- (uint64_t)priorityForAmounts:(NSArray *)amounts withAges:(NSArray *)ages;

// the block height after which the transaction can be confirmed without a fee, or TX_UNCONFIRMED for never
- (uint32_t)blockHeightUntilFreeForAmounts:(NSArray *)amounts withBlockHeights:(NSArray *)heights;

@end
