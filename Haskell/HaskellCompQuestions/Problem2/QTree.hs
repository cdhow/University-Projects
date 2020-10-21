{-# LANGUAGE ScopedTypeVariables #-}

module Main where
import Control.Monad (replicateM_, replicateM)


data QTree =
      Node QTree QTree QTree QTree
    | White
    | Black
    deriving (Show)

makeTree :: String -> QTree
makeTree s = fst $ makeTree' s

makeTree' :: String -> (QTree, String)
makeTree' ""  = error "Empty tree error"
makeTree' ('e':cs) = (White, cs)
makeTree' ('f':cs) = (Black, cs)
makeTree' ('p':cs) = 
    let (st1, cs1) = makeTree' cs
        (st2, cs2) = makeTree' cs1
        (st3, cs3) = makeTree' cs2
        (st4, cs4) = makeTree' cs3
    in (Node st1 st2 st3 st4, cs4)


-- Takes two QTrees and constructs a new union QTree
treeUnion :: QTree -> QTree -> QTree
treeUnion _ Black = Black
treeUnion Black _ = Black
treeUnion t White = t
treeUnion White t = t
treeUnion (Node a b c d) (Node a' b' c' d') =
     Node (treeUnion a a') (treeUnion b b') (treeUnion c c') (treeUnion d d')

-- Wrapper function for pixelCount' recursive function
pixelCount :: QTree -> Int
pixelCount tree = pixelCount' tree 1


-- Takes a QuadTree and a divisor and returns the number of pixels in the QuadTree
pixelCount' :: QTree -> Int -> Int
pixelCount' Black d = 1024 `div` d
pixelCount' White _ = 0
pixelCount' (Node st1 st2 st3 st4) d = 
      (pixelCount' st1 (d*4)) + (pixelCount' st2 (d*4)) 
    + (pixelCount' st3 (d*4)) + (pixelCount' st4 (d*4)) -- The divisor grows by *4 on each level


run :: IO ()
run = do
    [t, t'] <- replicateM 2 getLine
    let qt1 = makeTree t
    let qt2 = makeTree t'
    let res = pixelCount $ treeUnion qt1 qt2
    putStrLn ("There are " ++ (show res) ++ " black pixels.")


main :: IO ()
main = do
    numberOfOperations :: Int <- read <$> getLine

    replicateM_ numberOfOperations run