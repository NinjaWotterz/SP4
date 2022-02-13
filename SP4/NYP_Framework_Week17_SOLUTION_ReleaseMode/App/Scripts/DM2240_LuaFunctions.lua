local function replace(str, what, with)
    what = string.gsub(what, "[%(%)%.%+%-%*%?%[%]%^%$%%]", "%%%1") -- escape pattern
    with = string.gsub(with, "[%%]", "%%%%") -- escape replacement
    return string.gsub(str, what, with)
end

-- SetToLuaFile
function SetToLuaFile(arrNewKeyAndValue, overwrite, arrOldKeyAndValue)
     --File
     local f;
     --Wipe the contents with new data.
     if overwrite == 1 then
          --Open the file for overwriting.
          f = assert(io.open("Scripts//DM2240.lua", "w"))
          f:write(arrNewKeyAndValue)
          --Close the file.
          f:close()
          --Append with new data.
     elseif overwrite == 0 then
          --Open the file for appending.
          f = assert(io.open("Scripts//DM2240.lua", "a"))
          f:write(arrNewKeyAndValue)
          f:write("\n")
          --Close the file.
          f:close()
     elseif overwrite == 2 then
          --Open the file for reading.
          local file = assert(io.open("Scripts//DM2240.lua", "r"))
          --Read the entire file into memory.
          local fileContent = file:read("*all")
          --Close the file.
          file:close()

          --Remove special characters before replacement
          arrOldKeyAndValue = replace(arrOldKeyAndValue, "-", "%-")
          --Replace the relevant value in the entire content.
          fileContent = string.gsub(fileContent, arrOldKeyAndValue, arrNewKeyAndValue)

          file = assert(io.open("Scripts//DM2240.lua", "w"))
          --Write the entire content the file.
          file:write(fileContent)
          --Close the file.
          file:close()
     end
end

-- Calculate Distance Square
function CalculateDistanceSquare(x1,y1,z1,x2,y2,z2)
     --Calculate distance squared
     local distanceSquare = (x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1) + (z2 - z1) * (z2 - z1)
     --Print the result
     print(distanceSquare)
     -- Return the result
     return distanceSquare
end

-- Get the Min, Max, Average and number of values for a set of arguments
function GetMinMax(...)
     local minValue = -1
     local maxValue = -1
     local avgValue = -1
     local numValues = 0

     for k,v in ipairs{...} do
          if minValue == -1 then
               minValue = v
          elseif v < minValue then
               minValue = v
          end

          if maxValue == -1 then
               maxValue = v
          elseif v > maxValue then
               maxValue = v
          end

          avgValue = avgValue + v
          numValues = numValues + 1
     end

     avgValue = avgValue / numValues
     return minValue, maxValue, avgValue, numValues
end
