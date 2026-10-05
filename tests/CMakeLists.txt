# SPDX-FileCopyrightText: 2026 Volker Krause <vkrause@kde.org>
# SPDX-License-Identifier: BSD-3-Clause

add_executable(backgroundtest backgroundtest.cpp)
target_link_libraries(backgroundtest PRIVATE KF6SonnetCore Qt6::Widgets)

add_executable(languageguesstest languageguesstest.cpp)
target_link_libraries(languageguesstest PRIVATE KF6SonnetCore Qt6::Widgets)
