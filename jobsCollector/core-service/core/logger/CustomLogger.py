import logging
import json, re

class ColoredFormatter(logging.Formatter):
    COLORS = {
        'DEBUG': '\033[94m',  # Синий
        'INFO': '\033[92m',   # Зеленый
        'WARNING': '\033[93m',# Желтый
        'ERROR': '\033[91m',  # Красный
        'CRITICAL': '\033[95m' # Фиолетовый
    }
    RESET = '\033[0m'  # Сброс цвета

    def __init__(self, fmt=None, datefmt=None, style='%'):
        super().__init__(fmt, datefmt, style)

    def format(self, record):
        log_color = self.COLORS.get(record.levelname, self.RESET)
        record.msg = f"{log_color}{record.msg}{self.RESET}"
        return super().format(record)