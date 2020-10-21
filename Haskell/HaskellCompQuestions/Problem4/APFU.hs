import Data.List (nub, find, delete)
import Data.Maybe (catMaybes, isJust)
import Control.Monad (replicateM)

type Socket = String
type Device = String
data Adapter = Adapter String String deriving (Eq, Show)

-- Takes the list of adapters and expands it to
-- include chaining of adapters.
chainAdapters :: [Adapter] -> [Adapter]
chainAdapters as 
    | length newAdapters == length as = newAdapters -- no new chained adapters
    | otherwise                      = chainAdapters newAdapters -- find more chaining possibilities
        where
        newAdapters = nub $ as ++ chained -- add the new chained adapters to the old adapters (remove duplicates)
        chained = catMaybes [x `chain` y | x <- as, y <- as, x /= y] -- Get the new chained adapters

-- Takes two adapters and returns a new adapter
--  if a new adapter can be built from the two, else Nothing.
chain :: Adapter -> Adapter -> Maybe Adapter
chain (Adapter f t) (Adapter f' t')
    | t  == f'  = Just $ Adapter f t'
    | t' == f   = Just $ Adapter f t'
    | otherwise = Nothing


-- Takes a list of adapters, sockets and a device. Returns
-- the list of sockets that the device can be plugged in to.
getValidConnections :: [Adapter] -> [Socket] -> Device -> [Socket]
getValidConnections as ss dev = filter (isValidFit as dev) ss -- get the list of valid fitting sockets for dev

-- Takes the list of adapters, and checks if a device can fit in that socket,
-- either on it's own or with an adapter
isValidFit :: [Adapter] -> Socket -> Device -> Bool
isValidFit as d s = d == s || (or $ map (\(Adapter f t) -> s == t && d == f) as) 

solve :: [Adapter] -> [Socket] -> [Device] ->  Int
solve _ _ [] = 0 -- No more devices (base case)
solve as ss (d:ds)
    | length connectedSockets == 0 = 1 + solve as ss ds -- No valid connections, therefore +1 and recur
    | otherwise                    = minimum solutions -- else take the smallest solution
      where
        solutions = map (\ss' -> solve as ss' ds) socketRemovedLists -- Recursively try to connect the remaining devices to the new 2D socketRemoved lists.
        socketRemovedLists = map (\connectedSocket -> delete connectedSocket ss) connectedSockets -- get a 2D list of sockets, each list has a different connected socket removed
        connectedSockets = getValidConnections as ss d


parseAdapter :: IO Adapter
parseAdapter = do
  [a, b] <- words <$> getLine
  pure $ Adapter a b

main :: IO ()
main = do
  -- Get sockets
  socketsNum <- read <$> getLine
  ss <- replicateM socketsNum getLine

  -- Get Devices
  devicesNum <- read <$> getLine
  ds <- replicateM devicesNum (last . words <$> getLine)

  -- Get adapters
  adaptersNum <- read <$> getLine
  as <- replicateM adaptersNum parseAdapter

  -- Solve and print solution
  print $ solve (chainAdapters as) ss ds