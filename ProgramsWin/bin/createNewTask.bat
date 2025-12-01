@echo off
Setlocal EnableDelayedExpansion

SET task_path=Tasks
SET red_path=RedScenarios
SET blue_path=BlueScenarios

SET iterator=1

SET task_prefix=simple_task-0
SET red_prefix=simple_red_scn-0
SET blue_prefix=simple_blue_scn-0


:label1 if not exist %task_path%\%task_prefix%!iterator!.tas (
  if not exist %red_path%\%red_prefix%!iterator!.scn (
     if not exist %blue_path%\%blue_prefix%!iterator!.scn (
      cd %task_path%
      copy nul %task_prefix%!iterator!.tas
      echo //-----------------------------  Основные параметры ----------------------------- >> !task_prefix!!iterator!.tas
      echo                                           //  Подключаемые файлы >> !task_prefix!!iterator!.tas
      echo RedScene.1 = "%red_prefix%!iterator!.scn"          //  Файл сценария за красных >> !task_prefix!!iterator!.tas
      echo BlueScene.1 = "%blue_prefix%!iterator!.scn"          //  Файл сценария за синих >> !task_prefix!!iterator!.tas
      echo UseEnvironment = "Стандартный.env"                //  Файл условий применения >> !task_prefix!!iterator!.tas
      echo                                           //  Запись фильмов >> !task_prefix!!iterator!.tas
      echo film_count = 1                            //  Количество записываемых фильмов >> !task_prefix!!iterator!.tas
      cd ..
      cd %red_path%
      copy nul %red_prefix%!iterator!.scn
      echo hash = "null" >> !red_prefix!!iterator!.scn
      echo who_i_am = "RED" >> !red_prefix!!iterator!.scn
      echo geo_cs = "true" >> !red_prefix!!iterator!.scn
      cd ..
      cd %blue_path%
      copy nul %blue_prefix%!iterator!.scn
      echo hash = "null" >> !blue_prefix!!iterator!.scn
      echo who_i_am = "BLUE" >> !blue_prefix!!iterator!.scn
      echo geo_cs = "true" >> !blue_prefix!!iterator!.scn
      goto label2
      )
    )
  ) else (
      set/a iterator+=1
      goto label1
    )
:label2 cd ..
echo %cd%