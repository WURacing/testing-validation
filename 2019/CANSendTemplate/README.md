# CAN Send Template

The intention for this project is to have standard code designed to send CAN bus packets at any desired frequency. This project will be as stripped down as possible in order to maintain consistent output rate. This will be used in conjunction with an SD test to test actual performance of the BUS.

## Design Spec

| Data Logger Compatibility | Status |
| ----- | ----- |
| Configurable to operate at any bit rate via a single global variable | TODO |

| Performance/Reliability | Status |
| ----- | ----- |
| Operates with absolute minimum overhead | TODO |
| Utilizes Delta Timing | TODO |


| General Features | Status |
| ----- | ----- |
| Packets with both Standard and Extended ID's ought to be supported | TODO |
| Any number of packets ought to be able to be sent per `series` | TODO |
| Within a series, packets ought to share a `seriesID` as the last two bytes of the `data` body | TODO |
| Any packet send frequency ought to be supported via alteration of a single global variable | TODO |
| Time to send last series ought to be optionally the third from last byte in the data body | TODO |