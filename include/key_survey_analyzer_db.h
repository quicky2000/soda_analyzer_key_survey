/*
  This file is part of osm_diff_analyzer_key_survey, Openstreetmap
  diff analyzer based on CPP diff representation. It's aim is to survey
  editions concerning a specified tag
  Copyright (C) 2013  Julien Thevenon ( julien_thevenon at yahoo.fr )

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>
*/
#ifndef _KEY_SURVEY_ANALYZER_DB_H_
#define _KEY_SURVEY_ANALYZER_DB_H_

#include "osm_api_data_types.h"
#include "quicky_exception.h"
#include <set>
#include <iostream>
#include <cassert>

namespace osm_diff_analyzer_key_survey
{

  class key_survey_analyzer_db
  {
  public:
    template<typename T>
    inline bool is_interesting_id(const T & p_object)const;
    template<typename T>
    inline void store_interesting_id(const T & p_object);

  private:
    std::set<osm_api_data_types::osm_object::t_osm_id> m_interesting_node_ids;
    std::set<osm_api_data_types::osm_object::t_osm_id> m_interesting_way_ids;
    std::set<osm_api_data_types::osm_object::t_osm_id> m_interesting_relation_ids;
  };

  //----------------------------------------------------------------------------
  template<typename T>
    bool key_survey_analyzer_db::is_interesting_id(const T & p_object)const
    {
      throw quicky_exception::quicky_logic_exception("Should never be executed",__LINE__,__FILE__);
    }
  
  //----------------------------------------------------------------------------
  template<typename T>
    void key_survey_analyzer_db::store_interesting_id(const T & p_object)
    {
      throw quicky_exception::quicky_logic_exception("Should never be executed",__LINE__,__FILE__);
    }


  //----------------------------------------------------------------------------
  template<>
    inline bool key_survey_analyzer_db::is_interesting_id<osm_api_data_types::osm_node>(const osm_api_data_types::osm_node & p_object)const
    {
      return m_interesting_node_ids.find(p_object.get_id()) != m_interesting_node_ids.end();
    }
  
  //----------------------------------------------------------------------------
  template<>
    inline bool key_survey_analyzer_db::is_interesting_id<osm_api_data_types::osm_way>(const osm_api_data_types::osm_way & p_object)const
    {
      return m_interesting_way_ids.find(p_object.get_id()) != m_interesting_way_ids.end();
    }
  
  //----------------------------------------------------------------------------
  template<>
    inline bool key_survey_analyzer_db::is_interesting_id<osm_api_data_types::osm_relation>(const osm_api_data_types::osm_relation & p_object)const
    {
      return m_interesting_relation_ids.find(p_object.get_id()) != m_interesting_relation_ids.end();
    }
  
  //----------------------------------------------------------------------------
  template<>
    inline void key_survey_analyzer_db::store_interesting_id<osm_api_data_types::osm_node>(const osm_api_data_types::osm_node & p_object)
    {
      m_interesting_node_ids.insert(p_object.get_id());
    }
  
  //----------------------------------------------------------------------------
  template<>
    inline void key_survey_analyzer_db::store_interesting_id<osm_api_data_types::osm_way>(const osm_api_data_types::osm_way & p_object)
    {
      m_interesting_way_ids.insert(p_object.get_id());
    }
  
  //----------------------------------------------------------------------------
  template<>
    inline void key_survey_analyzer_db::store_interesting_id<osm_api_data_types::osm_relation>(const osm_api_data_types::osm_relation & p_object)
    {
      m_interesting_relation_ids.insert(p_object.get_id());
    }
  

  
}
#endif // _KEY_SURVEY_ANALYZER_DB_H_
//EOF
