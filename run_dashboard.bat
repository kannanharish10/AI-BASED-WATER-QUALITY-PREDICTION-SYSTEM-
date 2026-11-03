@echo off
title AI Water Quality and Potability Intelligence Dashboard
cd /d "%~dp0"

echo ========================================================
echo   AI Water Quality and Potability Intelligence System
echo ========================================================
echo.
echo [1/3] Project directory: %CD%

:: Check Python executable
set "PYTHON_EXE="
if exist "%LOCALAPPDATA%\Programs\Python\Python313\python.exe" (
    set "PYTHON_EXE=%LOCALAPPDATA%\Programs\Python\Python313\python.exe"
) else (
    where py >nul 2>&1
    if not errorlevel 1 (
        set "PYTHON_EXE=py"
    ) else (
        where python >nul 2>&1
        if not errorlevel 1 (
            set "PYTHON_EXE=python"
        )
    )
)

if "%PYTHON_EXE%"=="" (
    echo [ERROR] Python was not found on your system!
    echo Please install Python 3.13 and add it to PATH.
    pause
    exit /b 1
)

echo [2/3] Python engine: %PYTHON_EXE%
echo [3/3] Starting Streamlit server on http://localhost:8501 ...
echo.
echo Dashboard URL: http://localhost:8501
echo (Keep this terminal open while using the dashboard)
echo ========================================================
echo.

"%PYTHON_EXE%" -m streamlit run "AI_Water_Potability\app\dashboard.py" --server.port 8501 --server.address 127.0.0.1 --browser.gatherUsageStats false

pause

