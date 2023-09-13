local Grid = {}
Grid.__index = Grid

function Grid.new(tileSize, width, height)
    local self = setmetatable({}, Grid)
    self.width = width
    self.height = height
    self.tileSize = tileSize

    self.mt = {}
    for x = 0, self.width-1 do
        for y = 0, self.height-1 do
            self:set(x, y, 0)
        end
    end

    return self
end

function Grid:get(x, y)
    return self.mt[(math.floor(x) + math.floor(y) * self.width)]
end

function Grid:set(x, y, value)
    self.mt[(math.floor(x) + math.floor(y) * self.width)] = value
end

function Grid:reset()
    for x = 0, self.width-1 do
        for y = 0, self.height-1 do
            self:set(x, y, 0)
        end
    end
end

function Grid:each(callback)
    for x = 0, self.width-1 do
        for y = 0, self.height-1 do
            callback(x, y, self:get(x, y))
        end
    end
end


return Grid