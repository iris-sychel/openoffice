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



#ifndef ARY_IDL_IT_CE_HXX
#define ARY_IDL_IT_CE_HXX

// BASE CLASSES
#include <ary/idl/i_type.hxx>




namespace ary
{
namespace idl
{


/** A named ->Type related to its corresponding
     ->CodeEntity.
*/
class Ce_Type : public Type
{
  public:
    enum E_ClassId { class_id = 2201 };

    // LIFECYCLE
                        Ce_Type(
                            Ce_id               i_relatedCe,
                            const std::vector<Type_id> *
                                                i_templateParameters );
    virtual             ~Ce_Type();

    // INQUIRY
    Ce_id               RelatedCe() const       { return nRelatedCe; }

  private:
    // Interface csv::ConstProcessorClient:
    virtual void        do_Accept(
                            csv::ProcessorIfc & io_processor ) const;
    // Interface Object:
    virtual ClassId     get_AryClass() const;

    // Interface Type:
    virtual void        inq_Get_Text(
                            StringVector &      o_module,
                            String &            o_name,
                            Ce_id &             o_nRelatedCe,
                            int &               o_nSequemceCount,
                            const Gate &        i_rGate ) const;
    virtual const std::vector<Type_id> *
                        inq_TemplateParameters() const;
    // DATA
    Ce_id               nRelatedCe;
    Dyn< std::vector<Type_id> >
                        pTemplateParameters;
};




}   // namespace idl
}   // namespace ary
#endif
