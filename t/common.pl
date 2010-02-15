use Capture::Tiny 'capture';
use Carp;

sub no_err {
    err_like( $_[0], qr/^$/);
}

sub err_like {
    my ($stdout, $stderr);

    ($stdout, $stderr) = capture \&{$_[0]};
    like($stderr, $_[1]);
}

sub list_equal {
    my ($eq, $a, $b) = @_;

    croak "list_equal: \$a and \$b not lists"
      unless ref $a eq 'ARRAY' && ref $b eq 'ARRAY';

    return 0 unless @$a == @$b;          # compare lengths
    my @eq = map { &$eq ($a->[$_], $b->[$_]) } (0 .. $#$a);
    return 0 unless (grep ($_ == 1, @eq)) == @eq;
}

sub slist_equal {
    my ($a, $b) = @_;
    list_equal (sub {
                    my ($a, $b) = @_;
                    (defined $a && defined $b && $a eq $b) ||
                      (! defined $a && ! defined $b);
                }, $a, $b);
}

sub test_entry {
    my ($entry, $type, $key, $fields, $values) = @_;
    my ($i, @vals);

    croak "test_entry: num fields != num values"
      unless $#$fields == $#$values;
    ok($entry->parse_ok);
    is($entry->type, $type);

    if (defined $key) {
        is($entry->key, $key);
    } else {
        ok(!defined $entry->key);
    }

    ok(slist_equal ([$entry->fieldlist], $fields));
    for $i (0 .. $#$fields) {
        my $val = $entry->get ($fields->[$i]) || '';

        ok($entry->exists ($fields->[$i]));
        is($val, $values->[$i]);
    }

    @vals = map ($_ || '', $entry->get (@$fields));
    ok (slist_equal (\@vals, $values));
}

1;
