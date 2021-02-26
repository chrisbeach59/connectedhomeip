/*
 *
 *    Copyright (c) 2021 Project CHIP Authors
 *
 *    Licensed under the Apache License, Version 2.0 (the "License");
 *    you may not use this file except in compliance with the License.
 *    You may obtain a copy of the License at
 *
 *        http://www.apache.org/licenses/LICENSE-2.0
 *
 *    Unless required by applicable law or agreed to in writing, software
 *    distributed under the License is distributed on an "AS IS" BASIS,
 *    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *    See the License for the specific language governing permissions and
 *    limitations under the License.
 */

// THIS FILE IS GENERATED BY ZAP

#ifndef CHIP_CLUSTERS_H
#define CHIP_CLUSTERS_H

#import <Foundation/Foundation.h>

@class CHIPDevice;

typedef void (^ResponseHandler)(NSError * _Nullable error, NSDictionary * _Nullable values);

NS_ASSUME_NONNULL_BEGIN

/**
 * CHIPCluster
 *    This is the base class for clusters.
 */
@interface CHIPCluster : NSObject

- (nullable instancetype)initWithDevice:(CHIPDevice *)device endpoint:(uint8_t)endpoint queue:(dispatch_queue_t)queue NS_DESIGNATED_INITIALIZER;
- (instancetype)init NS_UNAVAILABLE;
+ (instancetype)new NS_UNAVAILABLE;

@end



/**
 * Cluster Basic
 *    
 */
@interface CHIPBasic : CHIPCluster

- (void)resetToFactoryDefaults:(ResponseHandler)completionHandler;

- (void)readAttributeZclVersion:(ResponseHandler)completionHandler;
- (void)readAttributeApplicationVersion:(ResponseHandler)completionHandler;
- (void)readAttributeStackVersion:(ResponseHandler)completionHandler;
- (void)readAttributeHardwareVersion:(ResponseHandler)completionHandler;
- (void)readAttributeManufacturerName:(ResponseHandler)completionHandler;
- (void)readAttributeModelIdentifier:(ResponseHandler)completionHandler;
- (void)readAttributeDateCode:(ResponseHandler)completionHandler;
- (void)readAttributePowerSource:(ResponseHandler)completionHandler;
- (void)readAttributeGenericDeviceClass:(ResponseHandler)completionHandler;
- (void)readAttributeGenericDeviceType:(ResponseHandler)completionHandler;
- (void)readAttributeProductCode:(ResponseHandler)completionHandler;
- (void)readAttributeProductUrl:(ResponseHandler)completionHandler;
- (void)readAttributeSwBuildId:(ResponseHandler)completionHandler;
- (void)readAttributeClusterRevision:(ResponseHandler)completionHandler;

@end



/**
 * Cluster Level Control
 *    
 */
@interface CHIPLevelControl : CHIPCluster

- (void)move:(uint8_t)moveMode rate:(uint8_t)rate optionMask:(uint8_t)optionMask optionOverride:(uint8_t)optionOverride completionHandler:(ResponseHandler)completionHandler;
- (void)moveToLevel:(uint8_t)level transitionTime:(uint16_t)transitionTime optionMask:(uint8_t)optionMask optionOverride:(uint8_t)optionOverride completionHandler:(ResponseHandler)completionHandler;
- (void)moveToLevelWithOnOff:(uint8_t)level transitionTime:(uint16_t)transitionTime completionHandler:(ResponseHandler)completionHandler;
- (void)moveWithOnOff:(uint8_t)moveMode rate:(uint8_t)rate completionHandler:(ResponseHandler)completionHandler;
- (void)step:(uint8_t)stepMode stepSize:(uint8_t)stepSize transitionTime:(uint16_t)transitionTime optionMask:(uint8_t)optionMask optionOverride:(uint8_t)optionOverride completionHandler:(ResponseHandler)completionHandler;
- (void)stepWithOnOff:(uint8_t)stepMode stepSize:(uint8_t)stepSize transitionTime:(uint16_t)transitionTime completionHandler:(ResponseHandler)completionHandler;
- (void)stop:(uint8_t)optionMask optionOverride:(uint8_t)optionOverride completionHandler:(ResponseHandler)completionHandler;
- (void)stopWithOnOff:(ResponseHandler)completionHandler;

- (void)readAttributeCurrentLevel:(ResponseHandler)completionHandler;
- (void) configureAttributeCurrentLevel:(uint16_t)minInterval  maxInterval:(uint16_t)maxInterval change:(uint8_t)change completionHandler:(ResponseHandler)completionHandler;
- (void) reportAttributeCurrentLevel:(ResponseHandler)reportHandler;
- (void)readAttributeRemainingTime:(ResponseHandler)completionHandler;
- (void)readAttributeOptions:(ResponseHandler)completionHandler;
- (void)writeAttributeOptions:(uint8_t)value completionHandler:(ResponseHandler)completionHandler;
- (void)readAttributeStartUpCurrentLevel:(ResponseHandler)completionHandler;
- (void)writeAttributeStartUpCurrentLevel:(uint8_t)value completionHandler:(ResponseHandler)completionHandler;
- (void)readAttributeClusterRevision:(ResponseHandler)completionHandler;

@end



/**
 * Cluster On/off
 *    
 */
@interface CHIPOnOff : CHIPCluster

- (void)off:(ResponseHandler)completionHandler;
- (void)offWithEffect:(uint8_t)effectId effectVariant:(uint8_t)effectVariant completionHandler:(ResponseHandler)completionHandler;
- (void)on:(ResponseHandler)completionHandler;
- (void)onWithRecallGlobalScene:(ResponseHandler)completionHandler;
- (void)onWithTimedOff:(uint8_t)onOffControl onTime:(uint16_t)onTime offWaitTime:(uint16_t)offWaitTime completionHandler:(ResponseHandler)completionHandler;
- (void)toggle:(ResponseHandler)completionHandler;

- (void)readAttributeOnOff:(ResponseHandler)completionHandler;
- (void) configureAttributeOnOff:(uint16_t)minInterval  maxInterval:(uint16_t)maxInterval completionHandler:(ResponseHandler)completionHandler;
- (void) reportAttributeOnOff:(ResponseHandler)reportHandler;
- (void)readAttributeGlobalSceneControl:(ResponseHandler)completionHandler;
- (void)readAttributeOnTime:(ResponseHandler)completionHandler;
- (void)writeAttributeOnTime:(uint16_t)value completionHandler:(ResponseHandler)completionHandler;
- (void)readAttributeOffWaitTime:(ResponseHandler)completionHandler;
- (void)writeAttributeOffWaitTime:(uint16_t)value completionHandler:(ResponseHandler)completionHandler;
- (void)readAttributeStartUpOnOff:(ResponseHandler)completionHandler;
- (void)writeAttributeStartUpOnOff:(uint8_t)value completionHandler:(ResponseHandler)completionHandler;
- (void)readAttributeClusterRevision:(ResponseHandler)completionHandler;

@end


NS_ASSUME_NONNULL_END

#endif /* CHIP_CLUSTERS_H */
