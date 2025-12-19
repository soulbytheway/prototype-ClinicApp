#include "stylesheet.h"

QString StyleHelper::getGlobalStyle() {
    return R"(
        /* --- ЗАГАЛЬНИЙ ФОН --- */
        QWidget {
            background-color: #2b2b2b;
            color: #f0f0f0;
            font-family: "Segoe UI", sans-serif;
            font-size: 14px;
        }

        /* --- ПОЛЯ ВВОДУ (Input Fields) --- */
        QLineEdit, QTextEdit, QPlainTextEdit {
            background-color: #3a3a3a;
            color: #ffffff;
            border: 1px solid #555;
            border-radius: 4px;
            padding: 6px;
            selection-background-color: #0078d7;
        }
        QLineEdit:focus, QTextEdit:focus {
            border: 1px solid #0078d7;
            background-color: #404040;
        }

        /* --- СПИСКИ ТА ТАБЛИЦІ --- */
        QTableWidget, QListWidget, QTableView {
            background-color: #3a3a3a;
            color: #ffffff;
            gridline-color: #555;
            border: 1px solid #555;
            selection-background-color: #0078d7;
            selection-color: white;
        }
        QHeaderView::section {
            background-color: #444;
            color: #ddd;
            padding: 6px;
            border: 1px solid #555;
            font-weight: bold;
        }
        QTableCornerButton::section {
            background-color: #444;
            border: 1px solid #555;
        }

        /* --- КНОПКИ (Buttons) --- */
        QPushButton {
            background-color: #0078d7;
            color: white;
            border: none;
            border-radius: 4px;
            padding: 8px 16px;
            font-weight: bold;
        }
        QPushButton:hover {
            background-color: #0063b1;
        }
        QPushButton:pressed {
            background-color: #004e8c;
        }
        QPushButton:disabled {
            background-color: #444;
            color: #888;
        }

        /* --- ГРУПИ (GroupBox) --- */
        QGroupBox {
            border: 1px solid #555;
            border-radius: 4px;
            margin-top: 20px;
            font-weight: bold;
            color: #0078d7;
        }
        QGroupBox::title {
            subcontrol-origin: margin;
            left: 10px;
            padding: 0 5px;
        }

        /* --- ВКЛАДКИ (Tabs) --- */
        QTabWidget::pane {
            border: 1px solid #444;
            background-color: #2b2b2b;
        }
        QTabBar::tab {
            background: #333;
            color: #aaa;
            padding: 8px 20px;
            border: 1px solid #444;
            border-bottom: none;
            border-top-left-radius: 4px;
            border-top-right-radius: 4px;
            margin-right: 2px;
        }
        QTabBar::tab:selected {
            background: #2b2b2b;
            color: #0078d7;
            border-bottom: 2px solid #0078d7;
            font-weight: bold;
        }
        QTabBar::tab:hover {
            background: #404040;
        }

        /* --- ВИПАДАЮЧІ СПИСКИ (ComboBox) --- */
        QComboBox {
            background-color: #3a3a3a;
            color: white;
            border: 1px solid #555;
            border-radius: 4px;
            padding: 6px;
        }
        QComboBox::drop-down {
            border: none;
            background: #444;
            width: 20px;
        }
        QComboBox QAbstractItemView {
            background-color: #3a3a3a;
            color: white;
            selection-background-color: #0078d7;
        }

        /* --- SCROLLBARS --- */
        QScrollBar:vertical {
            border: none;
            background: #2b2b2b;
            width: 10px;
            margin: 0px;
        }
        QScrollBar::handle:vertical {
            background: #555;
            min-height: 20px;
            border-radius: 5px;
        }
        QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {
            height: 0px;
        }
    )";
}
