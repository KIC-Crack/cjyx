################################################################################
#
#  Program: 3D Cjyx
#
#  Copyright (c) 2010 Kitware Inc.
#
#  See COPYRIGHT.txt
#  or http://www.slicer.org/copyright/copyright.txt for details.
#
#  Unless required by applicable law or agreed to in writing, software
#  distributed under the License is distributed on an "AS IS" BASIS,
#  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#  See the License for the specific language governing permissions and
#  limitations under the License.
#
#  This file was originally developed by Jean-Christophe Fillion-Robin, Kitware Inc.
#  and was partially funded by NIH grant 3P41RR013218-12S1
#
################################################################################

#CJyxMacroTranslation

#Parameters :
#
#   SRCS ..................: All sources witch need to be translated
#
#   UI_SRCS ...............: All ui_sources witch need to be translated
#
#   TS_DIR.................: Path to the TS files
#
#   TS_BASEFILENAME........: Name of the librairi
#
#   TS_LANGUAGES...........: Variable with all the languages
#
#   QM_OUTPUT_DIR_VAR .....: Translation's dirs generated by the macro
#
#   QM_OUTPUT_FILES_VAR....: Translation's files generated by the macro
#

function(CjyxMacroTranslation)
  set(options)
  set(oneValueArgs QM_OUTPUT_DIR_VAR TS_DIR TS_BASEFILENAME)
  set(multiValueArgs SRCS UI_SRCS TS_LANGUAGES QM_OUTPUT_FILES_VAR)
  cmake_parse_arguments(MY "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

  # ---------------------------------------------------------------------------------
  # Sanity Checks
  # ---------------------------------------------------------------------------------
  #set(expected_nonempty_vars SRCS UI_SRCS TS_INPUT)
#add TS_LANGUAGES also
  set(expected_nonempty_vars SRCS TS_DIR TS_BASEFILENAME )
  foreach(var ${expected_nonempty_vars})
    if("${MY_${var}}" STREQUAL "")
      message(FATAL_ERROR "error : ${var} Cmake varianle is empty !")
    endif()
  endforeach()

  # ---------------------------------------------------------------------------------
  # Set File to translate
  # ---------------------------------------------------------------------------------

  set(FILES_TO_TRANSLATE
    ${MY_SRCS}
    ${MY_UI_SRCS}
  )

  # ---------------------------------------------------------------------------------
  # Set TS_FILES
  # ---------------------------------------------------------------------------------
  set(TS_FILES)
  foreach(language ${MY_TS_LANGUAGES})
    set(ts_file "${MY_TS_DIR}${MY_TS_BASEFILENAME}_${language}.ts")

    if(NOT Cjyx_UPDATE_TRANSLATION AND NOT EXISTS ${ts_file})
      continue()
    endif()

    list(APPEND TS_FILES ${ts_file})
  endforeach()

  # ---------------------------------------------------------------------------------
  # UPDATE or ADD translation
  # ---------------------------------------------------------------------------------

    if(Cjyx_UPDATE_TRANSLATION)
      QT5_CREATE_TRANSLATION(QM_OUTPUT_FILES ${FILES_TO_TRANSLATE} ${TS_FILES})
    else()
      # Find existing TS files and only add translation if at least one translation file exist to avoid error
      # (Case may exist if Cjyx_UPDATE_TRANSLATION is disabled and translation files were never
      # generated for the input language)
      set(EXISTING_TS_FILES)
      foreach(TS_FILE ${TS_FILES})
        if(EXISTS ${TS_FILE})
          list(APPEND EXISTING_TS_FILES ${TS_FILE})
        endif()
      endforeach()

      if(EXISTING_TS_FILES)
        QT5_ADD_TRANSLATION(QM_OUTPUT_FILES ${EXISTING_TS_FILES})
      endif()
    endif()

  # ---------------------------------------------------------------------------------
  # Set the variable qm_output_dir
  # ---------------------------------------------------------------------------------
  # Extract the path associated with the first file of the list QM_OUTPUT_FILES
  # -> QM_OUTPUT_DIR

  list(GET QM_OUTPUT_FILES 0 QM_OUTPUT_FIRST_FILE)
  get_filename_component(qm_output_dir "${QM_OUTPUT_FIRST_FILE}" PATH)

  # ---------------------------------------------------------------------------------
  # Install language
  # ---------------------------------------------------------------------------------
  install(
    FILES ${QM_OUTPUT_FILES}
    DESTINATION ${Cjyx_INSTALL_QM_DIR}
    COMPONENT Runtime
  )

  # ---------------------------------------------------------------------------------
  # ADD custom command and target
  # ---------------------------------------------------------------------------------

  #add_custom_command(OUTPUT ${TS_FILES} DEPENDS ${FILES_TO_TRANSLATE} APPEND)
  #add_custom_target(TS_TRANSLATE DEPENDS ${MY_QM_FILES})

  # Output variables
  set(${MY_QM_OUTPUT_DIR_VAR} ${qm_output_dir} PARENT_SCOPE)
  set(${MY_QM_OUTPUT_FILES_VAR} ${QM_OUTPUT_FILES} PARENT_SCOPE)

endfunction()
