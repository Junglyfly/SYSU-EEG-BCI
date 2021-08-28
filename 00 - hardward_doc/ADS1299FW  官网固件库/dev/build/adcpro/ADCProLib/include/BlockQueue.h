
#ifndef GUARD_BlockQueue_hh
#define GUARD_BlockQueue_hh

#ifdef BLOCKQUEUE_TEST
#include <iostream>
#endif

class BlockQueue
{
#ifdef BLOCKQUEUE_TEST
    friend std::ostream& operator<<(std::ostream& o, BlockQueue& cr);
#endif

private:
    long* _start;         // beginning of queue memory
    long* _end;           // end of queue memory (adr of last elm + 1)
    long  _len;            // number of samples presently in queue
    long* _writePos;      // first available sample
    long* _newerWritePos; // latest reservation ptr
    long  _newerWriteLen;  // latest reservation length
    long* _olderWritePos; // next-to-latest reservation ptr
    long  _olderWriteLen;  // next-to-latest reservation length
    long* _readPos;       // read position
    long* _readEnd;       // temporary queue end (read wrap point)
    long  _pendingReadCount; // number of samples in reserved read
    /* When a write transaction is going on, this is the write 
    position.  _writePos is then the *old* write position.  We keep it 
    so a transaction can be cancelled.
    
    Usually this is the same as _writePos, but if we are wrapping it 
    may be _start instead */
    //long* _pendingWritePos; // position for a started write
    /* This is the write count for an open write transaction */
    //long _pendingWriteCount;

    long* pushReservation(long len);

    long* reservationStart()
    {
        if (_olderWritePos) return _olderWritePos;
        if (_newerWritePos) return _newerWritePos;
        if (_writePos==_start) return _readEnd;
        return _writePos;
    }

    long* reserveEnd(long& count);
    long* reserveStart(long& count);
    long* reserveMiddle(long& count);

public:
    BlockQueue(long* start, long* end);
    
    void clear();

    //! Reserve space for reading from the queue
    /*! Begins a read transaction for \p len bytes.
    
    This reserves as many contiguous bytes as possible.  If the requested 
    number of bytes cannot be delivered, \p len is adjusted.
    */
    long* startRead(long& len);
    void finishRead();
    long read(long* data, long count);
    
    //! Reserve space for writing to the queue
    /*! Begins a write transaction for \p len bytes.
    
    If there is space enough in the queue for a continuous block of \p len 
    samples, this returns a pointer to the beginning of the block, or
    null otherwise.
    
    When you finish writing, call finishWrite().  If you call startWrite() 
    again, the old write transaction is discarded and a new calculation is 
    made.
    
    To cancel the transaction completely, call this with \p len set to 0.
    */
    long* startWrite(long& len);
    void finishWrite();
    long write(long* data, long count);

    long getMaxLen() const { return _end-_start; }
    long len() const { return _len; }
    
    inline long* getReadPos(){ return _readPos; }
    inline long* getReadEnd(){ return _readEnd; }
    inline long* getStart(){ return _start; }
    
};

#endif
