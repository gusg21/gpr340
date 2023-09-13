local Boids = {
    _version = "0.1.0"
}

local Agent = require("agent")
local vector = require("vector")

Boids.all = {}

function Boids.init()
    --   local count = dbg.get("agent_count")
    local count = 1
    for i = 1, count do
        table.insert(Boids.all, Agent.new())
    end
end

local alignment_force = function(neighbors, self)
    local neighborDirectionSumVec = vector()
    local neighborCount = 0
    for i, neighbor in pairs(neighbors) do
        neighborDirectionSumVec:add(neighbor.velocity)
        neighborCount = neighborCount + 1
    end
    self.velocity:lerp(neighborDirectionSumVec / neighborCount, 0.1)
end

local separation_force = function(neighbors, self)
    -- body
end

local cohesion_force = function(neighbors, self)
    -- body
end

local avoid_tilemap = function(tilemap, self)
    local myPos = vector(self.position.x + self.velocity.x * 4, self.position.y + self.velocity.y * 4)
    local tilePos = (myPos / tilemap.tileSize):floor()
    print(tilemap:get(tilePos.x, tilePos.y))
    if tilemap:get(tilePos.x, tilePos.y) == 1 then
        self.velocity = self.velocity:rotate(0.1)
    end
end

-- updates a single boid:
local update_boid = function(dt, tilemap, neighbors, self)
    -- physics and stuff
    -- alignment_force(neighbors, self)
    separation_force(neighbors, self)
    cohesion_force(neighbors, self)

    avoid_tilemap(tilemap, self)

    -- update:
    self:update(dt)
end

function Boids.update(tilemap, dt)
    for i, agent in ipairs(Boids.all) do
        -- get local flock:
        local neighbors = {}
        for j, other in ipairs(Boids.all) do
            if i ~= j then
                local dist = vector.distance(agent.position, other.position)
                local radius = 20
                if radius >= dist then
                    table.insert(neighbors, other)
                end
            end
        end

        if agent == nil then
            print("null agent")
        end

        -- keeps the codebase clean:
        update_boid(dt, tilemap, neighbors, agent)
    end
end

return Boids
