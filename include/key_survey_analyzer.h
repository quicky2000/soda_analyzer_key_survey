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
#ifndef _KEY_SURVEY_ANALYZER_H_
#define _KEY_SURVEY_ANALYZER_H_

#include "cpp_analyzer_base.h"
#include "key_survey_analyzer_description.h"
#include "key_survey_analyzer_db.h"
#include "key_survey_stats.h"
#include "key_survey_common_api.h"
#include "module_configuration.h"
#include "quicky_exception.h"
#include <inttypes.h>
#include <map>
#include <fstream>
#include <cstdlib>
#include <sstream>
#include <set>
#include <iomanip>

namespace osm_diff_analyzer_key_survey
{
  class key_survey_common_api;

  class key_survey_analyzer:public osm_diff_analyzer_cpp_if::cpp_analyzer_base
  {
  public:
    key_survey_analyzer(const osm_diff_analyzer_if::module_configuration * p_conf,key_survey_common_api & p_api);
    ~key_survey_analyzer(void);
    // Methods inherited from cpp_analyzer_if
    void init(const osm_diff_analyzer_if::osm_diff_state * p_diff_state);
    void analyze(const std::vector<osm_api_data_types::osm_change*> & p_changes);
    const std::string & get_input_type(void)const;
    const std::string & get_output_type(void)const;
    const std::string & get_type(void)const;
    // End of inherited methods
  private:
    void create_report(void);
    inline bool compare(const osm_api_data_types::osm_core_element::t_osm_version p_previous_version,
			const osm_api_data_types::osm_node & p_node2,
			std::stringstream & p_report_detail,
                        const std::string & p_user_name,
                        const osm_api_data_types::osm_object::t_osm_id & p_user_id);
    inline bool compare(const osm_api_data_types::osm_core_element::t_osm_version p_previous_version,
			const osm_api_data_types::osm_way & p_way2,
			std::stringstream & p_report_detail,
                        const std::string & p_user_name,
                        const osm_api_data_types::osm_object::t_osm_id & p_user_id);
    inline bool compare(const osm_api_data_types::osm_core_element::t_osm_version p_previous_version,
			const osm_api_data_types::osm_relation & p_relation2,
			std::stringstream & p_report_detail,
                        const std::string & p_user_name,
                        const osm_api_data_types::osm_object::t_osm_id & p_user_id);
    inline bool compare(const osm_api_data_types::osm_core_element & p_core_element1,
                        const osm_api_data_types::osm_core_element & p_core_element2,
			std::stringstream & p_report_detail,
                        const std::string & p_user_name,
                        const osm_api_data_types::osm_object::t_osm_id & p_user_id);
    
    template <class T>
      void generic_analyze(const osm_api_data_types::osm_core_element * const p_object,
			   const osm_api_data_types::osm_change::t_osm_change_type & p_change);

    inline void display_object_html_link(std::ofstream & p_report,
                                         const osm_api_data_types::osm_core_element * const p_object);
    inline void display_user_html_link(std::ofstream & p_report,
                                       const std::string & p_user_name,
                                       const osm_api_data_types::osm_object::t_osm_id & p_user_id);
    inline void display_changeset_html_link(std::ofstream & p_report,
                                            const osm_api_data_types::osm_object::t_osm_id & p_changeset);

    key_survey_common_api & m_api;
    std::ofstream m_report;
    std::ofstream m_summary_report;
    std::string m_searched_string;
    key_survey_analyzer_db m_db;
    key_survey_stats m_stats;
    bool m_first_diff;
    std::string m_first_timestamp;
    std::string m_last_timestamp;
    static key_survey_analyzer_description m_description;
  };

  //------------------------------------------------------------------------------
  void key_survey_analyzer::display_object_html_link(std::ofstream & p_report,
                                                     const osm_api_data_types::osm_core_element * const p_object)
  {
    std::string l_object_url;
    m_api.get_object_browse_url(l_object_url,p_object->get_core_type_str(),p_object->get_id());
    p_report << "<A HREF=\"" << l_object_url << "\">" << p_object->get_core_type_str() << " " << p_object->get_id() << "</A>" ;
  }

  //------------------------------------------------------------------------------
  void key_survey_analyzer::display_user_html_link(std::ofstream & p_report,
                                                   const std::string & p_user_name,
                                                   const osm_api_data_types::osm_object::t_osm_id & p_user_id)
  {
    std::string l_user_url;
    m_api.get_user_browse_url(l_user_url,p_user_id,p_user_name);
    p_report << "<A HREF=\"" << l_user_url << "\">" << p_user_name << "</A>" ;
  }

  //------------------------------------------------------------------------------
  void key_survey_analyzer::display_changeset_html_link(std::ofstream & p_report,
                                                        const osm_api_data_types::osm_object::t_osm_id & p_changeset)
  {
    std::string l_changeset_url;
    m_api.get_object_browse_url(l_changeset_url,"changeset",p_changeset);
    p_report << "<A HREF=\"" << l_changeset_url << "\">" << "changeset " << p_changeset << "</A>" << std::endl ;
  }


  //------------------------------------------------------------------------------
  template <class T>
    void key_survey_analyzer::generic_analyze(const osm_api_data_types::osm_core_element * const p_object,
					       const osm_api_data_types::osm_change::t_osm_change_type & p_change)
    {

#ifndef FORCE_USE_OF_REINTERPRET_CAST
      const T * const l_casted_object = dynamic_cast<const T * const>(p_object);
#else
      const T * const l_casted_object = reinterpret_cast<const T * const>(p_object);
#endif // FORCE_USE_OF_REINTERPRET_CAST
 
      // Check if cast has been successfull
     if(l_casted_object==NULL)
        {
          std::stringstream l_stream;
          l_stream << "ERROR : invalid " << T::get_type_str() << " cast for object id " << p_object->get_id();
          throw quicky_exception::quicky_logic_exception(l_stream.str(),__LINE__,__FILE__);
        }

     // Check if the object has still been encountered and marked as interesting ( ie its contains the searched string in its tags )
     bool l_interesting = m_db.is_interesting_id(*l_casted_object);

      std::vector<std::string> l_matching_tags;

      if(!l_interesting)
        {
          // Get object tags
          const std::map<std::string,std::string> & l_tags = l_casted_object->get_tags();
      
          // Search if expected string appears in tag and store corresponding tags
          for(std::map<std::string,std::string>::const_iterator l_tag_iter = l_tags.begin();
              l_tag_iter != l_tags.end();
              ++l_tag_iter)
            {
              if(std::string::npos != l_tag_iter->first.find(m_searched_string))
                {
                  l_matching_tags.push_back(l_tag_iter->first);
                }
            }
          // If some tags corresponding to the seach have been found, remember object has interesting
          if(l_matching_tags.size())
            {
              m_db.store_interesting_id(*l_casted_object);
            }
        }

      // if object contains tags including the searched string then put the object in cache to reduce API use
      if(l_interesting || l_matching_tags.size())
	{
          // Store object in cache in case of creation or modification to reduce use of OSM API
          if(osm_api_data_types::osm_change::CREATION || osm_api_data_types::osm_change::MODIFICATION)
            {
	      //TO RESTABLISH              m_api.cache(*l_casted_object);
            }

          std::string l_change_description = "";
          std::stringstream l_report_detail;
          bool l_diff = true;
          switch(p_change)
            {
            case osm_api_data_types::osm_change::CREATION:
              l_change_description = "Creation";
              l_report_detail << "which following matching tag(s) :" << std::endl ;
              l_report_detail << "<ul>" << std::setprecision(9);
              for(std::vector<std::string>::const_iterator l_iter_tag = l_matching_tags.begin();
                  l_iter_tag != l_matching_tags.end();
                  ++l_iter_tag)
                {
                  l_report_detail << "<li>" << *l_iter_tag << " with value \"" << l_casted_object->get_tag_value(*l_iter_tag) << "\"</li>" << std::endl ;
                }
              l_report_detail << "</ul>" ;
              m_stats.object_creation(l_casted_object->get_user(),l_casted_object->get_user_id());
              break;
            case osm_api_data_types::osm_change::MODIFICATION:
            case osm_api_data_types::osm_change::DELETION:
              {
                if(p_change == osm_api_data_types::osm_change::DELETION)
                  {
                    m_stats.object_removed(l_casted_object->get_user(),l_casted_object->get_user_id());
                  }
                
                l_change_description = (p_change == osm_api_data_types::osm_change::MODIFICATION ? "Modification" : "Deletion");
                osm_api_data_types::osm_core_element::t_osm_version l_previous_version = l_casted_object->get_version() - 1;
                l_diff = compare(l_previous_version,*l_casted_object,l_report_detail,l_casted_object->get_user(),l_casted_object->get_user_id());
              }
              break;
            case osm_api_data_types::osm_change::INTERNAL_INVALID:
            default:
              {
                std::stringstream l_stream;
                l_stream << "ERROR : unexpected osm change value value \"" << p_change << "\"" ;
                throw quicky_exception::quicky_logic_exception(l_stream.str(),__LINE__,__FILE__);
              }
              break;
            }

          if(l_diff && !m_report.is_open())
            {
              create_report();                    
            }
          if(l_diff || l_report_detail.str() != "")
            {
              m_report << l_change_description << " of " ;
              display_object_html_link(m_report,l_casted_object);
              m_report << " by " ;
              display_user_html_link(m_report,l_casted_object->get_user(),l_casted_object->get_user_id());
              m_report << " in " ;
              display_changeset_html_link(m_report,l_casted_object->get_changeset());
              m_report << (std::string)l_report_detail.str();
              m_report << "<HR/>" << std::endl ;
            }
        
        }

   
    }

  //------------------------------------------------------------------------------
  bool key_survey_analyzer::compare(const osm_api_data_types::osm_core_element::t_osm_version p_previous_version,
				    const osm_api_data_types::osm_node & p_node2,
				    std::stringstream & p_report_detail,
                                       const std::string & p_user_name,
                                       const osm_api_data_types::osm_object::t_osm_id & p_user_id)
  {
    bool l_diff = false;
    const osm_api_data_types::osm_node * l_node1 = m_api.get_node_version(p_node2.get_id(),p_previous_version);
    bool l_redacted = false;
    osm_api_data_types::osm_core_element::t_osm_version l_redacted_version = p_previous_version - 1;
    if(l_node1 == NULL)
      {
        l_redacted = true;
        while( (l_node1 = m_api.get_node_version(p_node2.get_id(),l_redacted_version))==NULL && l_redacted_version)
          {
            --l_redacted_version;
          }
      }

    if(l_node1 != NULL)
      {
        if(l_redacted)
          {
            std::string l_redacted_link ;
            m_api.get_api_object_url(l_redacted_link,p_node2.get_core_type_str(),p_node2.get_id(),l_redacted_version);            
            p_report_detail << "<B>Due to redaction process comparison is done against <A HREF=\""<< l_redacted_link << "\">" << p_node2.get_core_type_str() << " " << p_node2.get_id() << " v" << l_redacted_version << "</A></B><BR>" << std::endl ;
	      //TO RESTABLISH            m_api.cache(p_node2);
          }

        l_diff = compare(*((const osm_api_data_types::osm_core_element *)l_node1),
                         *((const osm_api_data_types::osm_core_element *)&p_node2),
                         p_report_detail,
                         p_user_name,
                         p_user_id);
        delete l_node1;
      }
    else
      {
        p_report_detail << "<B>Unable to perform comparison because object had been redacted</B><BR>" << std::endl ;
      }
    return l_diff;
  }

  //------------------------------------------------------------------------------
  bool key_survey_analyzer::compare(const osm_api_data_types::osm_core_element::t_osm_version p_previous_version,
				    const osm_api_data_types::osm_way & p_way2,
				    std::stringstream & p_report_detail,
                                       const std::string & p_user_name,
                                       const osm_api_data_types::osm_object::t_osm_id & p_user_id)
  {
    bool l_diff = false;
    const osm_api_data_types::osm_way * l_way1 = m_api.get_way_version(p_way2.get_id(),p_previous_version);
    bool l_redacted = false;
    osm_api_data_types::osm_core_element::t_osm_version l_redacted_version = p_previous_version - 1;
    if(l_way1 == NULL)
      {
        l_redacted = true;
        while( (l_way1 = m_api.get_way_version(p_way2.get_id(),l_redacted_version))==NULL && l_redacted_version)
          {
            --l_redacted_version;
          }
      }

    if(l_way1 != NULL)
      {
        if(l_redacted)
          {
            std::string l_redacted_link ;
            m_api.get_api_object_url(l_redacted_link,p_way2.get_core_type_str(),p_way2.get_id(),l_redacted_version);            
            p_report_detail << "<B>Due to redaction process comparison is done against <A HREF=\""<< l_redacted_link << "\">" << p_way2.get_core_type_str() << " " << p_way2.get_id() << " v" << l_redacted_version << "</A></B><BR>" << std::endl ;
	      //TO RESTABLISH            m_api.cache(p_way2);
          }

        l_diff = compare(*((const osm_api_data_types::osm_core_element *)l_way1),
                         *((const osm_api_data_types::osm_core_element *)&p_way2),
                         p_report_detail,
                         p_user_name,
                         p_user_id);
        delete l_way1;
      }
    else
      {
        p_report_detail << "<B>Unable to perform comparison because object had been redacted</B><BR>" << std::endl ;
      }
    return l_diff;
  }

  //------------------------------------------------------------------------------
  bool key_survey_analyzer::compare(const osm_api_data_types::osm_core_element::t_osm_version p_previous_version,
				    const osm_api_data_types::osm_relation & p_relation2,
				    std::stringstream & p_report_detail,
                                       const std::string & p_user_name,
                                       const osm_api_data_types::osm_object::t_osm_id & p_user_id)
  {
    bool l_diff = false;
    const osm_api_data_types::osm_relation * l_relation1 = m_api.get_relation_version(p_relation2.get_id(),p_previous_version);
    bool l_redacted = false;
    osm_api_data_types::osm_core_element::t_osm_version l_redacted_version = p_previous_version - 1;
    if(l_relation1 == NULL)
      {
        l_redacted = true;
        while( (l_relation1 = m_api.get_relation_version(p_relation2.get_id(),l_redacted_version))==NULL && l_redacted_version)
          {
            --l_redacted_version;
          }
      }

    if(l_relation1 != NULL)
      {
        if(l_redacted)
          {
            std::string l_redacted_link ;
            m_api.get_api_object_url(l_redacted_link,p_relation2.get_core_type_str(),p_relation2.get_id(),l_redacted_version);            
            p_report_detail << "<B>Due to redaction process comparison is done against <A HREF=\""<< l_redacted_link << "\">" << p_relation2.get_core_type_str() << " " << p_relation2.get_id() << " v" << l_redacted_version << "</A></B><BR>" << std::endl ;
	      //TO RESTABLISH            m_api.cache(p_relation2);
          }

        l_diff = compare(*((const osm_api_data_types::osm_core_element *)l_relation1),
                         *((const osm_api_data_types::osm_core_element *)&p_relation2),
                         p_report_detail,
                         p_user_name,
                         p_user_id);
        delete l_relation1;
      }
    else
      {
        p_report_detail << "<B>Unable to perform comparison because object had been redacted</B><BR>" << std::endl ;
      }
    return l_diff;
  }

  //------------------------------------------------------------------------------
  bool key_survey_analyzer::compare(const osm_api_data_types::osm_core_element & p_core_element1,
				    const osm_api_data_types::osm_core_element & p_core_element2,
				    std::stringstream & p_report_detail,
                                       const std::string & p_user_name,
                                       const osm_api_data_types::osm_object::t_osm_id & p_user_id)
  {
    bool l_diff = false;
    bool l_first = true;
    // Loop on all tags of previous version of object
    for(std::map<std::string,std::string>::const_iterator l_iter = p_core_element1.get_tags().begin();
	l_iter != p_core_element1.get_tags().end();
	++l_iter)
      {
        // If tags contains the search string then check if it is still present in new version of has been modified
        if(std::string::npos != l_iter->first.find(m_searched_string))
          {
            if(p_core_element2.contains_tag(l_iter->first))
              {
                std::string l_new_value = p_core_element2.get_tag_value(l_iter->first);
                if(l_new_value != l_iter->second)
                  {
                    if(l_first) p_report_detail << "<ul>" << std::endl ; l_first = false;
                    p_report_detail << "<li>Tag \"" << l_iter->first << "\" value has changed from \"" << l_iter->second << "\" to \"" << l_new_value << "\"</li>" << std::endl ;
                    m_stats.tag_modified(p_user_name,p_user_id,p_core_element1.get_id(),p_core_element1.get_core_type());
                    l_diff = true;
                  }
              }
            else
              {
                if(l_first) p_report_detail << "<ul>" << std::endl ; l_first = false;
                p_report_detail << "<li>Tag \"" << l_iter->first << "\" with value \"" << l_iter->second << "\" has been removed</li>" << std::endl ;
                m_stats.tag_removed(p_user_name,p_user_id,p_core_element1.get_id(),p_core_element1.get_core_type());
                l_diff = true;
              }
          }
      }
    for(std::map<std::string,std::string>::const_iterator l_iter = p_core_element2.get_tags().begin();
	l_iter != p_core_element2.get_tags().end();
	++l_iter)
      {
        // If tags contains the search string then check if it has been added from the previous version
        if(std::string::npos != l_iter->first.find(m_searched_string))
          {
            if(!p_core_element1.contains_tag(l_iter->first))
              {
                if(l_first) p_report_detail << "<ul>" << std::endl ; l_first = false;
                p_report_detail << "<li>Tag \"" << l_iter->first << "\" with value \"" << l_iter->second << "\" has been added</li>" << std::endl ;
                m_stats.tag_added(p_user_name,p_user_id,p_core_element1.get_id(),p_core_element1.get_core_type());
                l_diff = true;
              }
          }
      }
    if(!l_first)
      {
        p_report_detail << "</ul>" << std::endl ;
      }
    return l_diff;
  }
}
#endif
