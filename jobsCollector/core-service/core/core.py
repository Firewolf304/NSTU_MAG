# system
import logging
from pathlib import Path

# local
from config.globals import get_global, set_global, global_var
from config.coreconf import CoreConfig

from .logger.CustomLogger import ColoredFormatter
from .logger.logger import logger

class core:
    config : CoreConfig = None
    config_path : str = "config.json"
    
    def __init__(self, config_path : str = "config.json") -> None:
        self.config_path = config_path
    
    def run (self) -> core:
        self.__init_logger__()
        self.__set_config__()
        self.__init_globals__()
        return self
    
    def __init_logger__ (self) -> None:
        filelogger = logging.FileHandler(self.config.LOGGER.FILENAME)
        filelogger.setLevel(logging.DEBUG)
        filelogger.mode = "w+"
        
        ch = logging.StreamHandler()
        level = getattr(logging, self.config.LOGGER.LOGLEVEL)
        format = self.config.LOGGER.FORMAT
        
        ch.setFormatter(ColoredFormatter(format))
        
        logging.basicConfig(level=level, format=format, handlers=[filelogger, ch])
        logger.info("Starting service")
    
    def __set_config__ (self, self_config : CoreConfig = None) -> None:
        path = None
        if (self.config_path):
            path = Path(self.config_path)
        if (self_config):
            self.config = self_config
        else:
            if not path.exists()
                logger.debug(f"No config file in {path.name}. Init base config")
                if path.parent.exists():    
        
        pass
    
    def __init_globals__ (self) -> None:
        set_global("config", self.config_path)
        pass