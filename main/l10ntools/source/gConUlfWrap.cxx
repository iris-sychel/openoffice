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
#include "gConUlf.hxx"



/*****************************************************************************
 *********************   G C O N X C S W R A P . C X X   *********************
 *****************************************************************************
 * This includes the c code generated by flex
 *****************************************************************************/



/**********************   I M P L E M E N T A T I O N   **********************/
namespace UlfWrap
{
#include "gConUlf_yy.c"
}



/**********************   I M P L E M E N T A T I O N   **********************/
void convert_ulf_impl::runLex()
{
  UlfWrap::genulf_lex();
}



/**********************   I M P L E M E N T A T I O N   **********************/
void convert_ulf_impl::setKey(std::string& sCollectedText)
{
  if (mbMergeMode)
    writeSourceFile(msCollector+sCollectedText);
  msCollector.clear();

  // locate key (is any)
  msKey = sCollectedText.substr(1,sCollectedText.size()-2);
}



/**********************   I M P L E M E N T A T I O N   **********************/
void convert_ulf_impl::setText(std::string& sCollectedText)
{
  int         nL, nE;
  std::string sText;


  if (mbMergeMode)
    writeSourceFile(msCollector+sCollectedText);
  msCollector.clear();

  // isolate text
  nL = sCollectedText.find("\"");
  if (nL == (int)std::string::npos)
	return;
  nE = sCollectedText.rfind("\"");
  sText = sCollectedText.substr(nL+1,nE - nL -1);

  if (mbMergeMode)
  {
    // get all languages (includes en-US)
    std::vector<l10nMem_entry *>& cExtraLangauges = mcMemory.getLanguagesForKey(msKey);
    std::string                   sNewLine;
    nL = cExtraLangauges.size();

	// and all other languages for that key
	for (int i = 0; i < nL; ++i)
    {
      sNewLine = cExtraLangauges[i]->msLanguage + " = \"" +
                 cExtraLangauges[i]->msText +
                 "\"\n";

      writeSourceFile(sNewLine);
    }
  }
  else
    mcMemory.setEnUsKey(msKey, msCollector);
}



/**********************   I M P L E M E N T A T I O N   **********************/
void convert_ulf_impl::collectData(std::string& sCollectedText)
{
  msCollector += sCollectedText;
  if (sCollectedText == "\n")
  {
	if (mbMergeMode)
      writeSourceFile(msCollector);
    msCollector.clear();
  }
}