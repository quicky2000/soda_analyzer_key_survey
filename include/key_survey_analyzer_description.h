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
#ifndef _KEY_SURVEY_ANALYZER_DESCRIPTION_H_
#define _KEY_SURVEY_ANALYZER_DESCRIPTION_H_

#include "analyzer_description_if.h"

namespace osm_diff_analyzer_key_survey
{
  class key_survey_analyzer_description: public osm_diff_analyzer_if::analyzer_description_if
  {
  public:
    const std::string & get_input_type(void)const;
    const std::string & get_output_type(void)const;
    const std::string & get_type(void)const;
  private:
    static const std::string m_input_type;
    static const std::string m_output_type;
    static const std::string m_type;
  };
}
#endif // _KEY_SURVEY_ANALYZER_DESCRIPTION_H_
//EOF
