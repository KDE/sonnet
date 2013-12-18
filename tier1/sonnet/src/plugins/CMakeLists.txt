
# you can find find_package() in kdelibs/cmake/modules/OptionalFindPackage.cmake
# it is the same as FIND_PACKAGE(<name>) but additionally creates an OPTION(WITH_<name>)
# so the checking for the software can be disabled via ccmake or -DWITH_<name>=OFF
find_package(ASPELL)
set_package_properties(ASPELL PROPERTIES
    URL "http://aspell.net/"
    DESCRIPTION "Spell checking support via Aspell"
    TYPE OPTIONAL
    PURPOSE "This is not needed for spell checking if Enchant is provided or only Hebrew spell checking is required"
)
if (ASPELL_FOUND)
   add_subdirectory( aspell )
endif ()


find_package(HSPELL)
set_package_properties(HSPELL PROPERTIES
    URL "http://ivrix.org.il/projects/spell-checker/"
    DESCRIPTION "Spell checking support for Hebrew"
    TYPE OPTIONAL
    PURPOSE "Hebrew support can also be provided via Enchant, providing the correct Enchant backends are installed"
)
if (HSPELL_FOUND)
   add_subdirectory( hspell )
endif ()


find_package(ENCHANT)
set_package_properties(ENCHANT PROPERTIES
    URL "http://www.abisource.com/projects/enchant/"
    DESCRIPTION "Spell checking support via Enchant"
    TYPE OPTIONAL
    PURPOSE ""
)
if (ENCHANT_FOUND)
   add_subdirectory( enchant )
endif ()

find_package(HUNSPELL)
set_package_properties(HUNSPELL PROPERTIES
    URL "http://hunspell.sourceforge.net/"
    DESCRIPTION "Spell checking support via Hunspell"
    TYPE OPTIONAL
    PURPOSE "This is not needed for spell checking if Enchant or Aspell is provided or only Hebrew spell checking is required."
)
if (HUNSPELL_FOUND)
   add_subdirectory( hunspell )
endif ()

