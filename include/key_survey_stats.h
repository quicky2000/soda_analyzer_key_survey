/*
    This file is part of osm_api_data_types, C++ representation of Openstreetmap
    API 0.6 primitive objects
    Copyright (C) 2012  Julien Thevenon ( julien_thevenon at yahoo.fr )

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
#ifndef _KEY_SURVEY_STATS_H_
#define _KEY_SURVEY_STATS_H_

#include "osm_api_data_types.h"
#include "inttypes.h"
#include <map>
#include <string>

namespace osm_diff_analyzer_key_survey
{
  class key_survey_stats
  {
  public:
    inline key_survey_stats(void);
    inline void object_creation(const std::string & p_user,
                                const osm_api_data_types::osm_object::t_osm_id & p_user_id);
    inline void tag_removed(const std::string & p_user,
                           const osm_api_data_types::osm_object::t_osm_id & p_user_id,
			   const osm_api_data_types::osm_object::t_osm_id & p_object_id,
			   const osm_api_data_types::osm_core_element::t_osm_type & p_object_type);
    inline void tag_added(const std::string & p_user,
                          const osm_api_data_types::osm_object::t_osm_id & p_user_id,
			   const osm_api_data_types::osm_object::t_osm_id & p_object_id,
			   const osm_api_data_types::osm_core_element::t_osm_type & p_object_type);
    inline void tag_modified(const std::string & p_user,
                             const osm_api_data_types::osm_object::t_osm_id & p_user_id,
			   const osm_api_data_types::osm_object::t_osm_id & p_object_id,
			   const osm_api_data_types::osm_core_element::t_osm_type & p_object_type);
    inline void object_removed(const std::string & p_user,
                               const osm_api_data_types::osm_object::t_osm_id & p_user_id);
    inline const uint32_t & get_object_creation(void)const;
    inline const uint32_t & get_tag_removed(void)const;
    inline const uint32_t & get_tag_added(void)const;
    inline const uint32_t & get_tag_modified(void)const;
    inline const uint32_t & get_object_removed(void)const;
    inline const uint32_t get_object_modified(void)const;
    inline const std::map<std::pair<osm_api_data_types::osm_object::t_osm_id ,std::string>,uint32_t>  & get_user_stats(void)const;
  private:
    inline void update_user_stats(const std::string & p_user,
                                  const osm_api_data_types::osm_object::t_osm_id & p_user_id);
    inline void update_object_stats(const osm_api_data_types::osm_object::t_osm_id & p_object_id,
				    const osm_api_data_types::osm_core_element::t_osm_type & p_object_type);
    uint32_t m_object_creation;
    uint32_t m_tag_removed;
    uint32_t m_tag_added;
    uint32_t m_tag_modified;
    std::set<std::pair<osm_api_data_types::osm_object::t_osm_id,osm_api_data_types::osm_core_element::t_osm_type> > m_object_modified;
    uint32_t m_object_removed;
    std::map<std::pair<osm_api_data_types::osm_object::t_osm_id ,std::string>,uint32_t> m_user_stats;
  };

  //----------------------------------------------------------------------------
  key_survey_stats::key_survey_stats(void):
    m_object_creation(0),
    m_tag_removed(0),
    m_tag_added(0),
    m_tag_modified(0),
    m_object_removed(0)
      {
      }

  //----------------------------------------------------------------------------
  void key_survey_stats::object_creation(const std::string & p_user,
                         const osm_api_data_types::osm_object::t_osm_id & p_user_id)
    {
      update_user_stats(p_user,p_user_id);
      ++m_object_creation;
    }

  //----------------------------------------------------------------------------
    void key_survey_stats::tag_removed(const std::string & p_user,
				       const osm_api_data_types::osm_object::t_osm_id & p_user_id,
				       const osm_api_data_types::osm_object::t_osm_id & p_object_id,
				       const osm_api_data_types::osm_core_element::t_osm_type & p_object_type)
    {
      update_user_stats(p_user,p_user_id);
      update_object_stats(p_object_id,p_object_type);
      ++m_tag_removed;
    }
  //----------------------------------------------------------------------------
    void key_survey_stats::tag_added(const std::string & p_user,
				     const osm_api_data_types::osm_object::t_osm_id & p_user_id,
				     const osm_api_data_types::osm_object::t_osm_id & p_object_id,
				     const osm_api_data_types::osm_core_element::t_osm_type & p_object_type)
    {
      update_user_stats(p_user,p_user_id);
      update_object_stats(p_object_id,p_object_type);
      ++m_tag_added;
    }

    //----------------------------------------------------------------------------
    void key_survey_stats::tag_modified(const std::string & p_user,
                                        const osm_api_data_types::osm_object::t_osm_id & p_user_id,
					const osm_api_data_types::osm_object::t_osm_id & p_object_id,
					const osm_api_data_types::osm_core_element::t_osm_type & p_object_type)
    {
      update_user_stats(p_user,p_user_id);
      update_object_stats(p_object_id,p_object_type);
      ++m_tag_modified;
    }

    //----------------------------------------------------------------------------
    void key_survey_stats::object_removed(const std::string & p_user,
                                          const osm_api_data_types::osm_object::t_osm_id & p_user_id)
    {
      update_user_stats(p_user,p_user_id);
      ++m_object_removed;
    }

    //----------------------------------------------------------------------------
    void key_survey_stats::update_user_stats(const std::string & p_user,
                           const osm_api_data_types::osm_object::t_osm_id & p_user_id)
    {
      std::map<std::pair<osm_api_data_types::osm_object::t_osm_id ,std::string>,uint32_t>::iterator l_iter = m_user_stats.find(std::pair<osm_api_data_types::osm_object::t_osm_id ,std::string>(p_user_id,p_user));
      if(l_iter != m_user_stats.end())
        {
          ++(l_iter->second);
        }
        {
          m_user_stats.insert(std::map<std::pair<osm_api_data_types::osm_object::t_osm_id ,std::string>,uint32_t>::value_type(std::pair<osm_api_data_types::osm_object::t_osm_id ,std::string>(p_user_id,p_user),1));
        }
    }

    //----------------------------------------------------------------------------
    void key_survey_stats::update_object_stats(const osm_api_data_types::osm_object::t_osm_id & p_object_id,
					       const osm_api_data_types::osm_core_element::t_osm_type & p_object_type)
    {
	m_object_modified.insert(std::make_pair(p_object_id,p_object_type));;
    }

    //----------------------------------------------------------------------------
    const uint32_t & key_survey_stats::get_object_creation(void)const
      {
        return m_object_creation;
      }

    //----------------------------------------------------------------------------
    const uint32_t & key_survey_stats::get_tag_removed(void)const
      {
        return m_tag_removed;
      }
    //----------------------------------------------------------------------------
    const uint32_t & key_survey_stats::get_tag_added(void)const
      {
        return m_tag_added;
      }
    //----------------------------------------------------------------------------
    const uint32_t & key_survey_stats::get_tag_modified(void)const
      {
        return m_tag_modified;
      }
    //----------------------------------------------------------------------------
    const uint32_t & key_survey_stats::get_object_removed(void)const
      {
        return m_object_removed;
      }
    //----------------------------------------------------------------------------
    const uint32_t key_survey_stats::get_object_modified(void)const
      {
        return m_object_modified.size();
      }
    //----------------------------------------------------------------------------
    const std::map<std::pair<osm_api_data_types::osm_object::t_osm_id ,std::string>,uint32_t> & key_survey_stats::get_user_stats(void)const
      {
        return m_user_stats;
      }


}

#endif // _KEY_SURVEY_STATS_H_
// EOF
