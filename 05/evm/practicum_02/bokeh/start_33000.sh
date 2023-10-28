#!/bin/sh
bokeh serve --address 195.19.32.95 --port 33002 --allow-websocket-origin=195.19.32.95:33002 --show $1
