io.stdout:setvbuf("no")

local Grid = require("grid")
local Agent = require("agent")
local Vector = require("vector")
local Boids = require("boids")

local startTime = love.timer.getTime()
local tilemap = nil
local tileSize = 16

function love.load()
    Boids.init()
    tilemap = Grid.new(tileSize, love.graphics.getWidth() / tileSize, love.graphics.getHeight() / tileSize)
end

function love.update(dt)
    Boids.update(tilemap, dt)

    if mouseDown then
        local tx = math.floor(love.mouse.getX() / tileSize)
        local ty = math.floor(love.mouse.getY() / tileSize)
        tilemap:set(tx, ty, 1)
    end
end

function love.mousepressed(x, y, button)
    mouseDown = true
end

function love.mousereleased(x, y, button)
    mouseDown = false
end

function love.draw()
    love.graphics.clear()
    tilemap:each(function(x, y, value) 
        local t = love.timer.getTime() - startTime
        local xNorm = math.sin(x/tilemap.width + t)
        local yNorm = math.cos(y/tilemap.height + t)
        love.graphics.setColor(xNorm, yNorm, (1-(xNorm*yNorm)), 1)
        if value == 0 then
            love.graphics.rectangle("line", x * tileSize, y * tileSize, tileSize, tileSize)
        elseif value == 1 then
            love.graphics.rectangle("fill", x * tileSize, y * tileSize, tileSize, tileSize)
        end
    end)
end