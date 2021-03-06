#include <lib.h>
#include <kmm.h>
#include <lock.h>
#include <thread.h>
#include <x86.h>

struct lock*
lock_create(const char* name) {
    struct lock* lock;

    lock = kmalloc(sizeof(struct lock));
    if (lock == NULL)
        return NULL;

    lock->lk_name = strdup(name);
    if (lock->lk_name == NULL) {
        kfree(lock);
        return NULL;
    }

    lock->lk_owner = NULL;
    lock->lk_count = 0;

    lock->lk_wchan = wchan_create(lock->lk_name);
    if (lock->lk_wchan == NULL) {
        kfree(lock->lk_name);
        kfree(lock);
        return NULL;
    }

    spinlock_init(&lock->lk_lock);

    return lock;
}

void
lock_destroy(struct lock* lock) {
    assert(lock != NULL);

    spinlock_cleanup(&lock->lk_lock);
    wchan_destroy(lock->lk_wchan);
    kfree(lock->lk_owner);
    kfree(lock->lk_name);
    kfree(lock);
}

void
lock_acquire(struct lock* lock) {
    assert(lock != NULL);
    assert(thisthread->in_interrupt == false);

    bool iflag = cli();

    spinlock_acquire(&lock->lk_lock);
    if (!lock_holding(lock)) {

        while (lock->lk_count > 0)
            wchan_sleep(lock->lk_wchan, &lock->lk_lock);

        assert(lock->lk_count == 0);
        assert(lock->lk_owner == NULL);
        lock->lk_owner = thisthread;
    }

    lock->lk_count++;
    assert(lock_holding(lock));

    spinlock_release(&lock->lk_lock);

    ifx(iflag);
}

void
lock_release(struct lock* lock) {
    assert(lock != NULL);

    bool iflag = cli();

    spinlock_acquire(&lock->lk_lock);

    assert(lock_holding(lock));
    assert(lock->lk_count > 0);

    lock->lk_count--;
    if (lock->lk_count == 0) {
        lock->lk_owner = NULL;
        wchan_wakeone(lock->lk_wchan, &lock->lk_lock);
    }

    spinlock_release(&lock->lk_lock);

    ifx(iflag);
}

bool
lock_holding(struct lock* lock) {
    assert(lock != NULL);
    assert(thisthread != NULL);

    return lock->lk_owner != NULL ? lock->lk_owner == thisthread : false;
}
