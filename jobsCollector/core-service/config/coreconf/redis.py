from pydantic import BaseModel, Field

class RedisConfig(BaseModel):
    URL: str = "redis://localhost:6379"
    MAX_CONNECTIONS: int = 100
    ENCODING: str = "utf-8"
    DECODE_RESPONSES: bool = True
    SESSION_EXPIRE: int = 3600  # secs