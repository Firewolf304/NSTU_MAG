from pydantic import BaseModel, Field

class PostgresConfig(BaseModel):
    HOST: str = "127.0.0.1"
    PORT: str = "5432"
    USER: str = "admin"
    PASSWORD: str = "postgres"
    DBNAME: str = "database"
    PROTOCOL: str = "postgresql"
    CREATE: bool = True
    MIGRATE: bool = True
    SETDEFAULT: bool = False
    SECURECODE: str = ""
    
    ECHO: bool = False
    
    MAX_CONNECTIONS: int = 100
    MIN_CONNECTIONS: int = 5
    
    STATEMENT_CACHE_SIZE: int = 100
    COMMAND_TIMEOUT: int = 60
    CONNECT_TIMEOUT: int = 60
    
    @property
    def url(self) -> str:
        return f"{self.PROTOCOL}://{self.USER}:{self.PASSWORD}@{self.HOST}:{self.PORT}/{self.DBNAME}"

    @property
    def url_async(self) -> str:
        return f"{self.PROTOCOL}+asyncpg://{self.USER}:{self.PASSWORD}@{self.HOST}:{self.PORT}/{self.DBNAME}"
    
    @property
    def pool_config(self) -> dict:
        return {
            "min_size": self.MIN_CONNECTIONS,
            "max_size": self.MAX_CONNECTIONS,
            "command_timeout": self.COMMAND_TIMEOUT,
            "statement_cache_size": self.STATEMENT_CACHE_SIZE,
        }