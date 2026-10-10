# system
from sqlalchemy.ext.asyncio import AsyncSession, create_async_engine, async_sessionmaker
from sqlalchemy.orm import declarative_base

# local
import logging
from config.globals import get_global

logger = logging.getLogger(__name__)
config = get_global("config")

engine = create_async_engine(config.DATABASE.POSTGRESQL.url_async, echo=config.DATABASE.POSTGRESQL.ECHO, future=True)