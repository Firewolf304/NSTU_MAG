from pydantic import BaseModel, Field

class LoggerConfig(BaseModel):
    LOGLEVEL: str = "DEBUG"
    FORMAT: str = "[%(asctime)s][%(levelname)s]=>%(message)s (%(filename)s:%(lineno)d)"
    FILENAME: str = "fastapi.log"
    NO_LOG: list[str] = Field(default_factory=list)
    DUPLICATE_FILTERS_FILELOG: bool = False