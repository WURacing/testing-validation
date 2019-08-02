# Recieve Write Benchmark

The intention for this project is to determine actual throughput of the WUFR's CAN Bus + Data Logging System.

## Design Spec

| Data Logger Compatibility | Status |
| ----- | ----- |
| Configurable to operate at any bit rate via a single global variable | TODO |
| Should be able to accept both Standard and Extended IDs | TODO |

| Performance/Reliability | Status |
| ----- | ----- |
| Should be designed to  | TODO |


| General Features | Status |
| ----- | ----- |
| Data ought to be logged with no byte error | TODO |
| Data ought to be written in the currently accepted, most performant way | TODO |
| Data Logger should be configured to minimize packet loss | TODO |
| Metrics should exist (logged in place of bytes 1-4 of the packet `data`) to benchmark events like `f_write` or `f_sync` |