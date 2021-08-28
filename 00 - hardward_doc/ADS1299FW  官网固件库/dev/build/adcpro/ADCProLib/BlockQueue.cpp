
#include "BlockQueue.h"

#ifdef BLOCKQUEUE_TEST
#include <iostream>

using namespace std;
#endif

BlockQueue::BlockQueue(long* start, long* end)
    : _start(start),
    _end(end)
{
    clear();
}

long* BlockQueue::pushReservation(long len)
{
    _olderWritePos=_newerWritePos;
    _olderWriteLen=_newerWriteLen;
    _newerWritePos=_writePos;
    _newerWriteLen=len;

    _writePos+=len;
    if (_writePos>=_end) {
        _writePos=_start;
        _readEnd=_end;
    }
    return _newerWritePos;
}

void BlockQueue::clear()
{
    _len=0;
    _writePos=_readPos=_start;
    //_pendingWritePos=0;
    _pendingReadCount=0;
    //_pendingWriteCount=0;
    _readEnd=_end;
    _newerWritePos=_olderWritePos=0;
    _newerWriteLen=_olderWriteLen=0;
}

long* BlockQueue::reserveEnd(long& count)
{
    if ((_end-_writePos)<count) {
        count=0;
        return 0;
    }
    return pushReservation(count);
}

long* BlockQueue::reserveMiddle(long& count)
{
    if ((_readPos-_writePos)<count) {
        count=0;
        return 0;
    }
    return pushReservation(count);
}

long* BlockQueue::reserveStart(long& count)
{
    if ((_readPos-_start)<count) {
        count=0;
        return 0;
    }
    _readEnd=_writePos;
    _writePos=_start;
    return pushReservation(count);
}

long* BlockQueue::startWrite(long& count)
{
    if ((_writePos==_readPos)&&_len) {
        count=0;
        return 0;
    }
    
    if (_readPos>_writePos) {
        return reserveMiddle(count);
    }
    
    long* p;
    long c=count;
    p=reserveEnd(c);
    if (!p)
        p=reserveStart(count);
    else
        count=c;
    return p;
}

void BlockQueue::finishWrite()
{
    if (_olderWritePos) {
        _len+=_olderWriteLen;
        _olderWritePos=0;
        _olderWriteLen=0;
    } else {
        _len+=_newerWriteLen;
        _newerWritePos=0;
        _newerWriteLen=0;
    }
}

#if 0
long* BlockQueue::startWrite(long& count)
{
#if 0
    cout<<"###begin startWrite("<<count<<"):";
    cout<<" _writePos="<<_writePos;
    cout<<" _readEnd="<<_readEnd;
    cout<<" _readPos="<<_readPos;
    cout<<" _end="<<_end;
    cout<<" _start="<<_start;
    cout<<" _len="<<_len;
    cout<<"\n";
#endif

    if (count<0) return 0;
    else if (count==0) {
        _pendingWritePos=0;
        return 0;
    }
    count=(count>getMaxLen())?getMaxLen():count;

    if (_writePos==_readPos) {
        if (_len!=0) { // queue is full
            count=0;
            return 0;
        }
    } else if (_writePos<_readPos) {
        if (count>(_readPos-_writePos))
            count=_readPos-_writePos;
        _pendingWritePos=_writePos;
    }

    if (_writePos>=_readPos) {
        if ((_end-_writePos)>=count) { // don't wrap yet
            _readEnd=_end;
            _pendingWritePos=_writePos;
        } else if ((_readPos-_start)>count) { // wrap
            _readEnd=_writePos;
            _pendingWritePos=_start;
        } else { // no room - give up
            count=0;
            _pendingWritePos=0;
        }
    }

    _pendingWriteCount=count;
#if 0
    cout<<"###end startWrite("<<count<<"):";
    cout<<" _pendingWritePos="<<_pendingWritePos;
    cout<<" _len="<<_len;
    cout<<"\n";
#endif
    return _pendingWritePos;
}

void BlockQueue::finishWrite()
{
    if (!_pendingWritePos) return;
    if (_pendingWritePos<_writePos)
        _writePos=_pendingWritePos;
    else
        _writePos=_pendingWritePos+_pendingWriteCount;
    if (_writePos>=_end) _writePos=_start;
    _len+=_pendingWriteCount;
    _pendingWritePos=0;
}
#endif

long BlockQueue::write(long* data, long count)
{
    long* dest=startWrite(count);
    if (!dest||!count) return 0;
    for (long i=0; i<count; ++i)
        *dest++=data[i];
    finishWrite();
    return count;
}

long* BlockQueue::startRead(long& count)
{
    long* resStart=reservationStart();
    long* max;

    if (_len==0) {
        count=0;
        _pendingReadCount=0;
        return 0;
    }
    if (_readPos>=resStart)
        max=_readEnd;
    else
        max=resStart;
    if ((max-_readPos)<count)
        count=max-_readPos;

    _pendingReadCount=count;
    return _readPos;
}

void BlockQueue::finishRead()
{
    if (!_pendingReadCount) return;
    _len-=_pendingReadCount;
    _readPos+=_pendingReadCount;
    if (_readPos>=_readEnd) _readPos=_start;
    _pendingReadCount=0;
}

long BlockQueue::read(long* data, long count)
{
    if (count<0) count=_len;
    long* src=startRead(count);
    if (!src||!count) return 0;
    for (long i=0; i<count; ++i)
        *data++=src[i];
    finishRead();
    return count;
}
