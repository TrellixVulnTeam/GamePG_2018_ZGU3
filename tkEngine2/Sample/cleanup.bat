@rem クリーンアップ
@setlocal

@set CURRENT_DIR=%~dp0
@pushd "%CURRENT_DIR%"
@for /d %%a in (.\*) do (call :deleteSymbolicLink "%%a")
@goto end

@rem シンボリックリンクの削除。
:deleteSymbolicLink
@set SAMPLE_FOLDER=%~1
@pushd "%~1"

@rmdir /s /q Game\Resource
@rmdir /s /q tkEngine
@rmdir /s /q tkTools

@popd

@exit /b


:end
@popd