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
#include <iostream>
#include "gLang.hxx"
#include <cstdlib>



/*****************************************************************************
 ************************   G H A N D L E R . C X X   ************************
 *****************************************************************************
 * This is the control module, that interpret the command line and implement
 * the different work types 
 *     extract / merge / generate / convert
 *****************************************************************************/



/**********************   I M P L E M E N T A T I O N   **********************/
handler::handler()
{
}



/**********************   I M P L E M E N T A T I O N   **********************/
handler::~handler()
{
}



/**********************   I M P L E M E N T A T I O N   **********************/
void handler::checkCommandLine(int argc, char *argv[])
{
  enum {ARG_NONE, ARG_F, ARG_O, ARG_S, ARG_T} eGotArg = ARG_NONE;
  std::string sWorkText, sLangText;
  int         argNow, nLen;
  bool        bKid = false;


  // make internal throw test (to avoid if cascades
  try
  {
    // check for fixed parameter: genLang <cmd> <module> <po dir> <languages>
    if (argc <= 5)
      throw "Not enough parameters";

    // check for working mode
    sWorkText = argv[1];
    if      (sWorkText == "convert")  meWorkMode = DO_CONVERT;
    else if (sWorkText == "extract")  meWorkMode = DO_EXTRACT;
    else if (sWorkText == "merge")    meWorkMode = DO_MERGE;
    else if (sWorkText == "generate") meWorkMode = DO_GENERATE;
    else if (sWorkText == "help")     showManual();
    else                              throw "<command> is mandatory";

    // and set fixed parameters
    msModuleName = argv[2];
    msPoOutDir   = msPoDir = argv[3];
    sLangText    = argv[4];
    if (sLangText[0] == '\"')
      sLangText.erase(0,1);
    nLen = sLangText.size() -1;
    if (sLangText[nLen] == '\"')
      sLangText.erase(nLen);
    if (!sLangText.size())
      throw "<languages> is mandatory";

    // and convert language to a vector
    {
      int current;
      int next = -1;
      do
      {
        current = next + 1;
        next = sLangText.find_first_of( ",", current );
        mvLanguages.push_back(sLangText.substr(current,next-current));
      }
      while (next != (int)std::string::npos);
    }
    
    // decode parameters and translate to variables
    for (argNow = 5; argNow < argc;)
    {
      std::string sArg(argv[argNow++]);

      // all -x is followed by a directory/file name
      if (sArg[0] == '-')
      {
        // Terminate collection ?
        if (eGotArg == ARG_F)
          eGotArg = ARG_NONE;

        // Are we waiting for a directory
        if (eGotArg != ARG_NONE)
          throw std::string("missing argument to ") + argv[argNow-1];

        // is it a known parameter
        if      (sArg == "-d") l10nMem::setShowDebug();  
        else if (sArg == "-f") eGotArg   = ARG_F;      
        else if (sArg == "-k") bKid      = true;      
        else if (sArg == "-o") eGotArg   = ARG_O;      
        else if (sArg == "-s") eGotArg   = ARG_S;      
        else if (sArg == "-t") eGotArg   = ARG_T;      
        else if (sArg == "-v") l10nMem::setShowVerbose();  
        else throw std::string("unknown parameter: ") + sArg;
      }
      else
      {
        switch (eGotArg)
        {
          case ARG_F: mvSourceFiles.push_back(sArg);           break;
          case ARG_O: msPoOutDir   = sArg; eGotArg = ARG_NONE; break;
          case ARG_S: msSourceDir  = sArg; eGotArg = ARG_NONE; break;
          case ARG_T: msTargetDir  = sArg; eGotArg = ARG_NONE; break;
       }
      }
    }

    // check parameters according to function
    if (!mvSourceFiles.size())
      throw "-f <files> is mandatory";

    // partly valid
    if (meWorkMode != DO_MERGE && msTargetDir.size())
      throw "-t is mandatory using \"extract\"";
    if (meWorkMode == DO_MERGE && !msTargetDir.size())
      throw "-t is only valid using \"extract\"";

    // Key Identification generation
    if (bKid)
    {
      if (meWorkMode != DO_MERGE)
        throw "-k is only valid with \"merge\"";
      else
      {
         meWorkMode = DO_MERGE_KID;
         sWorkText += "(KID)";
      }
    }
  }
  catch(const char *sErr)
  {
    showUsage(std::string(sErr));
    exit(-1);
  }
  catch(std::string sErr)
  {
    showUsage(sErr);
    exit(-1);
  }

  // update directories to include final /
  nLen = msSourceDir.size();
  if (nLen && msSourceDir.at(nLen-1) != '/')
    msSourceDir.append("/");
  nLen = msTargetDir.size();
  if (nLen && msTargetDir.at(nLen-1) != '/')
    msTargetDir.append("/");
  nLen = msPoDir.size();
  if (nLen && msPoDir.at(nLen-1) != '/')
    msPoDir.append("/");
  nLen = msPoOutDir.size();
  if (nLen && msPoOutDir.at(nLen-1) != '/')
    msPoOutDir.append("/");

  // tell system
  mcMemory.showVerbose("gLang starting to " + sWorkText + " in module " + msModuleName);
}



/**********************   I M P L E M E N T A T I O N   **********************/
void handler::run()
{
  try
  {
    // prepare translation memory to module type
    mcMemory.setModuleName(msModuleName);

    // build list of languages (to be loaded and later written
    loadL10MEM();

    // use workMode to start correct control part
    switch (meWorkMode)
    {
      case DO_EXTRACT:   runExtractMerge(false, false); break;
      case DO_MERGE:     runExtractMerge(true,  false); break;
      case DO_MERGE_KID: runExtractMerge(true,  true);  break;
      case DO_CONVERT:   runConvert();                  break;
      case DO_GENERATE:  runGenerate();                 break;
    }
  }
  catch(bool)
  {
    exit(-1);
  }
}



/**********************   I M P L E M E N T A T I O N   **********************/
void handler::runExtractMerge(bool bMerge, bool bKid)
{
  //JIX HANDLE KID

  // loop through all source files, and extract messages from each file
  for (std::vector<std::string>::iterator siSource = mvSourceFiles.begin(); siSource != mvSourceFiles.end(); ++siSource)
  {
    // tell system
    mcMemory.showDebug("gLang extracting text from file " + *siSource);

    // prepare translation memory
    mcMemory.setFileName(0, *siSource, true);

    // get converter and extract files
    convert_gen convertObj(mcMemory, msSourceDir + *siSource);
    convertObj.execute(bMerge);
  }

  // and generate language file
  mcMemory.save(msTargetDir + msModuleName);
}



/**********************   I M P L E M E N T A T I O N   **********************/
void handler::runConvert()
{
  throw mcMemory.showError(std::string("handler::runConvert not implemented"));
}



/**********************   I M P L E M E N T A T I O N   **********************/
void handler::runGenerate()
{
  throw mcMemory.showError(std::string("handler::runGenerate not implemented"));
}



/**********************   I M P L E M E N T A T I O N   **********************/
void handler::showUsage(std::string& sErr)
{
  // do we have an error text ?
  if (sErr.size())
    std::cerr << "commandline error:" << sErr << std::endl;

  std::cout <<
    "syntax oveview, use \"genLang help\" for full description\n"
    "genLang <cmd> <module> <po dir> <languages> [-d] [-f <files>] [-k] [-o <dir>] [-s <dir>] [-t <dir>] [-v]\n"
    "<cmd> is one of \"convert\", \"extract\", \"generate\", \"help\", \"merge\",\n";

  exit(-1);
}



/**********************   I M P L E M E N T A T I O N   **********************/
void handler::showManual()
{
  // give the correct usage
  std::cout <<
    "genLang (c)2013 by Apache Software Foundation\n"
    "=============================================\n"
    "As part of the L10N framework for Apache Open Office (AOO),\n"
    "genLang extracts en-US texts sources of the following types:\n"
    "  .xrm, .xhp, .xcu, .xcs, .ulf, .tree, .src, .prop and .po\n"
    "and merges with .po files in different languages.\n"
    "genLang merges .po files with AOO sources to add languages.\n"
    "\n"
    "genLang can also convert old .po files (generated from .sdf)\n"
    "\n"
    "Syntax:\n"
    "  genLang extract <module> <po dir> <languages> [-v] [-d]\\\n"
    "          [-o <po outdir>]  -f <files> -s <source dir>\n"
    "    reads <po dir>/*lang/<module>.po files and extract text\n"
    "    from <source dir>/<files>, result is merged and\n"
    "    written to <po outdir>/*lang/<module>.po if -o present\n"
    "    or <po dir>/*lang/<module>.po is overwritten\n"
    "    - File will only be written if they are changes\n"
    "    - Keys in .po files, not in sources files (deleted keys)\n"
    "      are moved to '-p <dir>'/<module>.deleted.po\n"
    "    - Keys in .po files, with changed text in the source\n"
    "      are marked \"fuzzy\"\n"
    "    - Keys in source files not in .po files (new keys)\n"
    "      are added and marked \"fuzzy\"\n"
    "\n"
    "  genLang merge <module> <po dir> <languages> [-v] [-d] [-k]\\\n"
    "          [-o <po outdir>]  -f <files> -s <source dir> \\\n"
    "          -t <target dir>\n"
    "    works as \"extract\" and additionally merges\n"
    "    <source dir>/<files> with all native language text\n"
    "    from <po dir>/*lang/<module>.po\n"
    "    The result is stored in <target dir>/<files>\n"
    "\n"
    "  genLang convert <module> <po dir> <languages> [-v] [-d]\\\n"
    "          [-o <po outdir>]  -f <files>\n"
    "    reads sdf generated .po <files> and merges with\n"
    "    <po dir>/*lang/<module>.po\n"
    "    Result is written to <po outdir>/*lang/<module>.po if\n"
    "    present or <po dir>/*lang/<module>.po is overwritten\n"
    "    - File will only be written if they are changes\n"
    "    - Keys in <files>, not in <module>.po\n"
    "      are moved to <po dir>/<module>.deleted.po\n"
    "    - Keys in <files>, with changed translation\n"
    "      are marked \"fuzzy\"\n"
    "\n"
    "  genLang generate <module> <po dir> <languages> [-v] [-d]\\\n"
    "          -o <po outdir>\n"
    "    reads .po <files> and generates a \"bin\" file for fast loading\n"
    "    Result is written to <po outdir>/<module>.dbpo\n"
    "\n"
    "  genLang help\n"
    "    this text\n"
    "\n"
    "Parameters:\n"
    "  <module>\n"
    "     name of module (directory in main)\n"
    "  <po dir>\n"
    "     directory containing a directory for each language which contains\n"
    "     a .po file for each module or a module.dbpo file for fast loading\n"
    "  <languages>\n"
    "     comma separated string with langauge id to be used\n"
    "\n"   
    "  -d\n"
    "     extensive verbose mode, tells what gLang is doing in detail\n";
    "  -f <files>\n"
    "     list of files containing messages to be extracted\n"
    "     \"convert\" expect sdf generated po files, to be converted\n"
    "  -k\n"
    "     generate kid id (hex) for all messages in the source code,\n"
    "     solely for QA\n"
    "  -o <po outdir>\n"
    "     directory to write .po files, same structure as -p\n"
    "  -s <source dir>\n"
    "     directory containing all source files (root path for <files>\n"
    "  -t <target dir>\n"
    "     <directory> used to write merged source files (root path for <files>\n"
    "  -v\n"
    "     verbose mode, tells what gLang is doing\n";

  exit(0);
}



/**********************   I M P L E M E N T A T I O N   **********************/
void handler::loadL10MEM()
{
  std::string sMod  = "/" +  msModuleName + ".pot";
  std::string sLoad = msPoDir + "../en-US" + sMod;


  // load texts from en-US po file (master)
  {
    // prepare translation memory
    mcMemory.setFileName(0, sLoad, true);

    // tell system
    mcMemory.showDebug("gLang loading master text from file " + sLoad);

    // and load file
    convert_gen(mcMemory, sLoad).execute(true);
  }

  // loop through all languages and load text
  for (std::vector<std::string>::iterator siLang = mvLanguages.begin(); siLang != mvLanguages.end(); ++siLang)
  {
    sLoad = msPoDir + *siLang + sMod;

    // prepare translation memory
    mcMemory.setFileName(0, sLoad, true);

    // tell system
    mcMemory.showDebug("gLang loading text from language file " + sLoad);

    // get converter and extract files
    convert_gen(mcMemory, sLoad).execute(false, true);
  }
}