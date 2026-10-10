# SERVICE
CORE-SERVICE:
    config
    db
    token collection
    api collector
    sheduler (cron-like)
    API health check (tokens by local + remote check)

WORKER:
    HTTP-requests (data miner)
    Rate-limitter 
    Retry logic for API and cache remote data (banned, data, etc)
    Query saver

WORKER-MINER (XML or raw text data-miner): 
    parsing data for analyzing 
    normalizing data
    tagging (may be)
    data extractor (salary)


# DB
REDIS
    TASK sheduler (no rabbit may be)
    SESSION/WORKER collector

PSQL
    DATA collector
    JOB collector
    PROVIDER collector
    HISTORY collector
    ANALYZE collector
    SUBSCRIPTION collector
    