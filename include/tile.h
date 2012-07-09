#ifndef TILE_H_
#define TILE_H_

#define MAX_NUMBER_OF_PLAYER 4

#define	NO_TILE  0
#define MJ_CHARACTOR_BASE  '0'
#define MJ_CIRCLE_BASE  '@'
#define MJ_BAMBOO_BASE  '`'
#define MJ_EAST  (MJ_BAMBOO_BASE+12)
#define MJ_SOUTH  (MJ_BAMBOO_BASE+15)
#define MJ_WEST  (MJ_BAMBOO_BASE+18)
#define MJ_NORTH  (MJ_BAMBOO_BASE+21)
#define MJ_ZHONG  (MJ_BAMBOO_BASE+24)
#define MJ_FA  (MJ_BAMBOO_BASE+27)
#define MJ_BAI  (MJ_BAMBOO_BASE+30)

typedef char tile_t;

#define MJ_CHARACTOR(n)    ((tile_t)(MJ_CHARACTOR_BASE + (n)))
#define C MJ_CHARACTOR
#define MJ_CIRCLE(n)    ((tile_t)(MJ_CIRCLE_BASE + (n)))
#define R MJ_CIRCLE
#define MJ_BAMBOO(n)    ((tile_t)(MJ_BAMBOO_BASE + (n)))
#define B MJ_BAMBOO

typedef int eaten_t;
#define PONG(tile)  (0x80 | (tile))
#define CHOW(tile)  (0x100 | (tile))

#endif /* TILE_H_ */
