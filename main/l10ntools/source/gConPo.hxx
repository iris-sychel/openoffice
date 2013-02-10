/**************************************************************
 * 
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 * 
 *   http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 * 
 *************************************************************/
#ifndef GCONPO_HXX
#define GCONPO_HXX
#include "gLang.hxx"
#include <stack>



/*****************************************************************************
 **************************   G C O N P O . H X X   **************************
 *****************************************************************************
 * This is the class header for .po conversion
 *****************************************************************************/
typedef enum {TAG_COMPONENT, TAG_PROP, TAG_NODE } TAG_TYPE;



/********************   C L A S S   D E F I N I T I O N   ********************/
class po_stack_entry
{
  public:
    po_stack_entry(TAG_TYPE sIsNode, std::string& sName);
    ~po_stack_entry();

    TAG_TYPE mbIsNode;
    std::string   msName;
};




/********************   C L A S S   D E F I N I T I O N   ********************/
class po_stack_entry;
class convert_po_impl : public convert_gen
{
  public:
    convert_po_impl(const std::string& srSourceFile, l10nMem& crMemory, bool brVerbose);
    ~convert_po_impl();

    void pushKeyPart(TAG_TYPE eIsNode, std::string &sTag);
    void popKeyPart (TAG_TYPE eIsNode, std::string &sTag);

    void startCollectData(std::string& sCollectedText);
    void stopCollectData(std::string& sCollectedText);
    void collectData(std::string& sCollectedText);

  private:
    std::stack<po_stack_entry> mcStack;
    bool                   mbMergeMode;
    bool                   mbCollectingData;
    std::string                 msCollector;

    void extract();
    void insert();
    void runLex();

    friend class convert_po;
};
#endif