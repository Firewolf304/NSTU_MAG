from typing import Any, Dict, Optional
from pathlib import Path
import json
from pydantic_settings import BaseSettings

from .postgres import *
from .redis import *
from .logger import *

class CoreConfig(BaseSettings):
    POSTGRES: postgres.PostgresConfig
    REDIS: redis.RedisConfig
    LOGGER: logger.LoggerConfig